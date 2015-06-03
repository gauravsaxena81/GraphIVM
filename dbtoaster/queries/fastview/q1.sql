INCLUDE 'examples/queries/fastview/schema.sql';

SELECT A.user_id, B.user_id, tweet_id, tweet_date FROM users A, tweet B WHERE A.user_id = B.user_id;
