SELECT title, rating FROM movies, ratings WHERE id = movie_id AND year = 2010 AND votes > 0
GROUP BY title
ORDER BY rating DESC;