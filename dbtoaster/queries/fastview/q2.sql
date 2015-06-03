INCLUDE 'examples/queries/fastview/schema.sql';

--SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id, tweet_id, tweet_date FROM follower A, tweet B WHERE A.user_id = B.user_id;
SELECT A.user_id, follower_id, B.user_id, tweet_id, tweet_date FROM follower A JOIN tweet B ON A.user_id = B.user_id;
