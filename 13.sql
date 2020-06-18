SELECT DISTINCT(people.name) FROM people, movies, stars WHERE movies.id IN
(SELECT movies.id FROM people, movies, stars WHERE people.name = "Kevin Bacon" 
AND people.id = stars.person_id AND people.birth = 1958 AND
movies.id = stars.movie_id) AND people.name != "Kevin Bacon"
AND movies.id = stars.movie_id AND stars.person_id = people.id;