CREATE STREAM FOLLOWER(
	user_id INT,
	follower_id INT
	)
  	FROM FILE 'examples/data/fastviewsmall/follower.csv' 
	LINE DELIMITED CSV();

CREATE STREAM RETWEET(
	user_id INT, 
	tweet_id INT, 
	tweet_date CHAR(20), 
	retweet_tweet_id INT
	)
  	FROM FILE 'examples/data/fastviewsmall/retweet.csv' 
	LINE DELIMITED CSV();

CREATE STREAM TWEET(
	user_id INT, 
	tweet_id INT, 
	tweet_date CHAR(20)
	)
  	FROM FILE 'examples/data/fastviewsmall/tweet.csv' 
	LINE DELIMITED CSV();

CREATE STREAM USERS(
	user_id INT
	)
  	FROM FILE 'examples/data/fastviewsmall/users.csv' 
	LINE DELIMITED CSV();

