INCLUDE 'examples/queries/fastview/schema.sql';

SELECT A.follower_id, count(tweet_id) FROM follower A, tweet B WHERE A.user_id = B.user_id group by A.follower_id;
