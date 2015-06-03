INCLUDE 'examples/queries/fastview/schema.sql';

SELECT A.user_id, count(tweet_id)  FROM users A, tweet B WHERE A.user_id = B.user_id group by A.user_id;
