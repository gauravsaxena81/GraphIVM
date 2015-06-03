INCLUDE 'examples/queries/fastview/schema.sql';

SELECT A.user_id as t_user_id, A.tweet_id, count(D.follower_id) FROM tweet A, retweet B, retweet C, follower D WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id
