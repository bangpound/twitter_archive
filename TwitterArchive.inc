<?php

class TwitterArchive extends Twitter {

  /**
   * Calls a Twitter API endpoint and throws the exception back to the caller.
   */
  public function callAndTrap($path, $params = array(), $method = 'GET') {
    $url = $this->create_url($path);

    try {
      $response = $this->auth_request($url, $params, $method);
    }
    catch (TwitterException $e) {
      watchdog('twitter', '!message', array('!message' => $e->__toString()), WATCHDOG_ERROR);
      throw $e;
      return FALSE;
    }

    if (!$response) {
      return FALSE;
    }

    return $this->parse_response($response);
  }
}
