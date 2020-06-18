SELECT movies.title FROM people, movies, ratings, stars WHERE people.name = "Chadwick Boseman" AND
people.id = stars.person_id AND movies.id = ratings.movie_id AND movies.id = stars.movie_id 
ORDER BY rating DESC
LIMIT 5;