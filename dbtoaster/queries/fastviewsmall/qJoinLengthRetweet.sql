
INCLUDE 'examples/queries/fastviewsmall/schema.sql';

SELECT A.tweet_id, Z.follower_id
FROM
retweet A
, retweet B
--, retweet C
--, retweet D
--, retweet E
, follower Z
WHERE
A.tweet_id = B.retweet_tweet_id
--and B.tweet_id = C.retweet_tweet_id
--and C.tweet_id = D.retweet_tweet_id
--and D.tweet_id = E.retweet_tweet_id
and B.user_id = Z.user_id;
