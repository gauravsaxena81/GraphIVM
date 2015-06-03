INCLUDE 'examples/queries/fastview/schema.sql';

SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id, B.retweet_tweet_id as r1_retweet_tweet_id, B.tweet_date as r1_tweet_date , C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id, D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, follower D WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id  and C.user_id = D.user_id
