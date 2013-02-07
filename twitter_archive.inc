<?php

/**
 * Determine if the Twitter user ID is authenticated.
 */
function twitter_archive_account_is_auth($twitter_uid) {
  if ($account = twitter_account_load($twitter_uid)) {
    return $account->is_auth();
  }
}

/**
 * Function to retrieve Twitter status using authorization if possible.
 */
function twitter_archive_statuses_show($id, $twitter_uid) {
  if ($twitter_uid && $account = twitter_account_load($twitter_uid)) {
    $twitter = twitter_archive_connect($account->is_auth() ? $account : NULL);
  }
  else {
    $twitter = twitter_archive_connect();
  }
  $params = array(
    'id' => $id,
    'include_entities' => 'true',
  );
  try {
    $status = $twitter->callAndTrap('statuses/show', $params, 'GET');
  }
  catch (TwitterException $e) {
    throw $e;
  }
  return $status;
}

/**
 * Connect to the Twitter API.
 *
 * @param object $account
 *   An authenticated twitter_account object to be used to authenticate against
 *   Twitter.
 * @return
 *   a Twitter object ready to be used to query the Twitter API or FALSE.
 */
function twitter_archive_connect($account = NULL) {
  if (!$account) {
    // Load the first authenticated account.
    $twitter_uid = db_select('twitter_account', 'ta')
      ->fields('ta', array('twitter_uid'))
      ->condition('oauth_token', '', '<>')
      ->condition('oauth_token_secret', '', '<>')
      ->execute()
      ->fetchField();
    $account = twitter_account_load($twitter_uid);
  }
  if ($account) {
    $auth = $account->get_auth();
    if (isset($auth['oauth_token']) && isset($auth['oauth_token_secret'])) {
      return new TwitterArchive(
        variable_get('twitter_consumer_key', ''),
        variable_get('twitter_consumer_secret', ''),
        $auth['oauth_token'],
        $auth['oauth_token_secret']
      );
    }
  }
  return FALSE;
}
