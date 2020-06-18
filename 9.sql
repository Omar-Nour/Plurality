-- BUGGY, SOME NAMES MISSING; OUTPUTS 17965 RATHER THAN 18013.

SELECT COUNT(DISTINCT(people.name)) FROM people, stars, movies WHERE movies.year = 2004 AND 
stars.movie_id = movies.id AND stars.person_id = people.id
--GROUP BY name {commented cuz it breaks the count()..}
ORDER BY birth ASC;

-- IDENTICAL RESULT : 17965
--SELECT COUNT(DISTINCT(name)) FROM people, stars, movies WHERE 
--movies.id IN (SELECT id FROM movies WHERE movies.year = 2004) AND movies.id = stars.movie_id AND stars.person_id = people.id
--ORDER BY people.birth ASC;

-- GuideLines:
--------------
--In 9.sql, write a SQL query to list the names of all people who starred in a movie released in 2004, 
--ordered by birth year.
--Your query should output a table with a single column for the name of each person.
--People with the same birth year may be listed in any order.
--No need to worry about people who have no birth year listed, 
--so long as those who do have a birth year are listed in order.
--If a person appeared in more than one movie in 2004, 
--they should only appear in your results once.
-- check results in a table with 1 column and 18,013 rows.
