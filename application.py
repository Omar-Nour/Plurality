import os
import datetime

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, toUsd, totalUp, symbs,toUsd2

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    if request.method == "GET":
        rows = db.execute("SELECT * FROM home WHERE uid = :uid ORDER BY symbol", uid=session["user_id"])
        ccell = db.execute("SELECT * FROM users WHERE id = :uid", uid=session["user_id"])[0]["cash"]

        # append cash data to each row
        for i in range(len(rows)):
            rows[i]["price"] = float(lookup(rows[i]["symbol"])["price"])
            rows[i]["total"] = rows[i]["price"]*rows[i]["shares"]

        total= usd(totalUp(rows,ccell))
        rows = toUsd(rows)
        ccell = usd(ccell)
        return render_template("index.html", rows=rows, cash=ccell, total=total)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        if int(request.form.get("shares")) < 1:
            return apology("Shares must be a positive number greater than 1")
        else:
            # POST method code:
            check = lookup(request.form.get("symbolb"))
            if check == None:
                return apology("Invalid Symbol")
            else:
                cash = float(db.execute("SELECT * FROM users WHERE id = :current", current=session["user_id"])[0]["cash"])
                price = float(check["price"]*int(request.form.get("shares")))
                if cash >= price:
                    updt_cash = float(cash - price)
                    # update cash in users account
                    db.execute("UPDATE users SET cash = :crnt WHERE id = :cuid", crnt=updt_cash, cuid=session["user_id"])

                    # record purchase in purchases table
                    db.execute("INSERT INTO purchases (uid, symbol, amount, price, date) VALUES (:uid, :symb, :amnt, :price, :date)",
                        uid=session["user_id"],
                        symb=check["symbol"],
                        amnt="+"+request.form.get("shares"),
                        price=check["price"],
                        date=datetime.datetime.now())

                    # record in index table
                    if check["symbol"] in symbs(session["user_id"],db):
                        oldshares = int(db.execute("SELECT * FROM home WHERE uid=:uid AND symbol= :symbol",uid=session["user_id"],symbol=check["symbol"])[0]["shares"])
                        db.execute("UPDATE home SET shares = :shares WHERE uid= :uid AND symbol= :symbol",
                        uid=session["user_id"],
                        symbol=check["symbol"],
                        shares=int(request.form.get("shares"))+oldshares)
                    else:
                        db.execute("INSERT INTO home (symbol, name, shares, uid) VALUES (:symbol, :name, :shares, :uid)",
                        symbol=check["symbol"],
                        name=check["name"],
                        shares=int(request.form.get("shares")),
                        uid=session["user_id"])


                    flash("Bought!")
                    return redirect("/")

                else:
                    return apology("Not enough cash :/")


@app.route("/history")
@login_required
def history():
    rows = toUsd2(db.execute("SELECT * FROM purchases WHERE uid = :uid UNION SELECT * FROM sales WHERE uid = :uid ORDER BY date",uid=session["user_id"]))
    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbolData = lookup(request.form.get("symbol"))
        if symbolData == None:
            return apology("Invalid Symbol.")
        else:
            return render_template("quoted.html", data=symbolData)


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")
    else:
        name = request.form.get("username")
        password = request.form.get("password")

        chk = db.execute("SELECT * FROM users WHERE username = :usrn", usrn=name)
        if len(chk) == 0:
            # hash password
            phash = generate_password_hash(password)
            db.execute("INSERT INTO users (username, hash) VALUES (:un, :phash)", un=name, phash=phash)
            uid = db.execute("SELECT * FROM users WHERE username = :usrn", usrn=name)[0]["id"]
            session["user_id"] = uid
            flash("Registered!")
            return redirect("/")
        else:
            return apology("Username Already Taken.")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # TOFIX
    if request.method == "GET":
        return render_template("sell.html")
    else:
        sell_s = request.form.get("sell_s")
        if sell_s in symbs(session["user_id"],db):
            cshares = int(db.execute("SELECT * FROM home WHERE uid = :uid AND symbol= :symbol", uid=session["user_id"], symbol=sell_s)[0]["shares"])
            if int(request.form.get("s_shares")) > cshares:
                return apology("You have less shares than you are selling")
            else:
                upshares = cshares - int(request.form.get("s_shares"))

                # update cash in users table
                crntcash = float(db.execute("SELECT * FROM users WHERE id = :current", current=session["user_id"])[0]["cash"])
                extrac = float(lookup(sell_s)["price"])*int(request.form.get("s_shares"))
                upcash = float(crntcash + extrac)
                db.execute("UPDATE users SET cash = :crnt WHERE id = :cuid", crnt=upcash, cuid=session["user_id"])

                # update shares in home table
                db.execute("UPDATE home SET shares = :shares WHERE uid = :uid AND symbol= :symbol", shares=upshares, uid=session["user_id"], symbol=sell_s)

                # Record sale in sales table
                db.execute("INSERT INTO sales (uid, symbol, amount, price, date) VALUES (:uid, :symb, :amnt, :price, :date)",
                        uid=session["user_id"],
                        symb=sell_s,
                        amnt="-"+request.form.get("s_shares"),
                        price=float(lookup(sell_s)["price"]),
                        date=datetime.datetime.now())

                flash("Sold!")
                return redirect("/")

        else:
            return apology("Not Purchased or Invalid")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)

@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("cash.html")
    else:
        cash = float(request.form.get("cash"))
        if cash <= 0.0:
            return apology("Must Enter a positive number")
        else:
            crncash = float(db.execute("SELECT * FROM users WHERE id= :uid",uid=session["user_id"])[0]["cash"])
            upcash = crncash + cash
            db.execute("UPDATE users SET cash = :crnt WHERE id = :cuid", crnt=upcash, cuid=session["user_id"])

            # Add cash as as sale transaction
            db.execute("INSERT INTO sales (uid, symbol, amount, price, date) VALUES (:uid, :symb, :amnt, :price, :date)",
                        uid=session["user_id"],
                        symb="ADDED CASH",
                        amnt="+$",
                        price=cash,
                        date=datetime.datetime.now())

            flash("Added cash!")
            return redirect("/")


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
