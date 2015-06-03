INCLUDE 'examples/queries/fastview/schema.sql';

select A.user_id as u_user_id, B.tweet_id, count(C.tweet_id) from users A, tweet B, retweet C where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id, B.tweet_id;
