SELECT DISTINCT(name) FROM people, ratings, directors WHERE rating >= 9.0 AND 
directors.movie_id = ratings.movie_id AND directors.person_id = people.id;
