INCLUDE 'examples/queries/fastview/schema.sql';

select A.user_id as f_user_id, B.user_id as t_user_id , B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id, C.tweet_date as r_tweet_date from users A, tweet B, retweet C where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id
