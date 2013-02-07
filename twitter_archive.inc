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
    $twitter = twitter_connect($account->is_auth() ? $account : NULL);
  }
  else {
    $twitter = twitter_connect();
  }
  $params = array(
    'id' => $id,
    'include_entities' => 'true',
  );
  return $twitter->call('statuses/show', $params, 'GET');
}
