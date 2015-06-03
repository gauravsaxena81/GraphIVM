CREATE STREAM FOLLOWER(
	user_id INT,
	follower_id INT
	)
  	FROM FILE 'examples/data/fastview/follower.csv' 
	LINE DELIMITED CSV();

CREATE STREAM RETWEET(
	user_id INT, 
	tweet_id INT, 
	tweet_date CHAR(20), 
	retweet_tweet_id INT
	)
  	FROM FILE 'examples/data/fastview/retweet.csv' 
	LINE DELIMITED CSV();

CREATE STREAM TWEET(
	user_id INT, 
	tweet_id INT, 
	tweet_date CHAR(20)
	)
  	FROM FILE 'examples/data/fastview/tweet.csv' 
	LINE DELIMITED CSV();

CREATE STREAM USERS(
	user_id INT
	)
  	FROM FILE 'examples/data/fastview/users.csv' 
	LINE DELIMITED CSV();

