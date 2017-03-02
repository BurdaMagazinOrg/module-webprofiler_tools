<?php

/**
 * Implements hook_drush_command().
 */
function webprofiler_tools_drush_command() {
  $commands = array();

  $commands['webprofiler-tools-inspect'] = [
    'description' => dt('Inspects a profile.'),
    'core' => ['8+'],
    'arguments' => [
      'token' => 'The profile token to inspect',
    ]
  ];
  return $commands;
}

function drush_webprofiler_tools_inspect($token) {
  /**
   * @var \Drupal\webprofiler\Profiler\DatabaseProfilerStorage $profiler_database_storage
   */
  $profiler_database_storage = Drupal::service('profiler.database_storage');
  $profile = $profiler_database_storage->read($token);
  /**
   * @var \Drupal\webprofiler\DataCollector\DatabaseDataCollector $database_collector
   */
  $database_collector = $profile->getCollector('database');
  $queries = $database_collector->getQueries();

  $query_pattern = [
    'total_time' => 0.00,
    'count' => 0,
    'queries' => [],
    'query' => '',
  ];
  $queries_aggregated = [];
  foreach ($queries as $query) {
    $query_key = md5($query['query']);
    if (!array_key_exists($query_key, $queries_aggregated)) {
      $queries_aggregated[$query_key] = $query_pattern;
      $queries_aggregated[$query_key]['query'] = $query['query'];
    }
    $queries_aggregated[$query_key]['queries'][] = $query;
    $queries_aggregated[$query_key]['count']++;
    $queries_aggregated[$query_key]['total_time'] += $query['time'];
  }

  $queries_aggregated_by_time = $queries_aggregated;
  uasort($queries_aggregated_by_time, function ($a, $b) { return strnatcmp($b['total_time'], $a['total_time']); });

  $query_time = 0.00;
  foreach ($queries_aggregated_by_time as $query) {
    $query_time += $queries_aggregated_by_time['total_time'];
  }

  $foo = 'bar';
}