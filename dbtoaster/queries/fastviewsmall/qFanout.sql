INCLUDE 'examples/queries/fastviewsmall/schema.sql';

SELECT A.follower_id, Z.tweet_id 
FROM  
follower A
, follower B
, tweet Z
WHERE A.user_id = B.follower_id
AND B.user_id = Z.user_id
;
