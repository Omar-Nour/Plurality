SELECT title FROM movies WHERE movies.id IN
(SELECT movies.id FROM movies, people, stars WHERE people.name = "Helena Bonham Carter" 
AND people.id = stars.person_id AND stars.movie_id = movies.id) AND 
movies.id IN (SELECT movies.id FROM movies, people, stars WHERE people.name = "Johnny Depp" 
AND people.id = stars.person_id AND stars.movie_id = movies.id);
