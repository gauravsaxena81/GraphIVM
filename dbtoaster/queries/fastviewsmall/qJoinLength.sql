INCLUDE 'examples/queries/fastviewsmall/schema.sql';

SELECT A.user_id, Z.tweet_id 
FROM  
follower A
, follower B 
, follower C
, follower D
, tweet Z
WHERE A.follower_id = B.user_id
and B.follower_id = C.user_id
and C.follower_id = D.user_id
and D.follower_id = Z.user_id;
