INCLUDE 'examples/queries/fastview/schema.sql';

select A.user_id as f_user_id, B.user_id as t_user_id , B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id, C.tweet_date as r_tweet_date, D.user_id from users A, tweet B, retweet C, users D where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id
