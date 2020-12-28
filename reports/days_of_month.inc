<?php

/**
 * @file
 * Days of month report for the visitors module.
 */

/**
 * Get data for days of month report order the query based on a header array.
 *
 * @param header
 *   Table header array. If header is NULL - data is not sorted.
 *
 * @return
 *   hours data array
 */
function visitors_days_of_month_data($header) {
  $query = db_select('visitors', 'v');
  $query->addExpression('COUNT(*)', 'count');
  $query->addExpression(
    visitors_date_format_sql('visitors_date_time', '%d'), 'day'
  );
  $query->groupBy('day');
  visitors_date_filter_sql_condition($query);

  if (!is_null($header))
    $query->extend('TableSort')->orderByHeader($header);

  return $query->execute();
}

/**
 * Display days of month report.
 *
 * @return
 *   string days of month report html source
 */
function visitors_days_of_month() {
  $header = array(
    array('data' => t('#')),
    array('data' => t('Day'), 'field' => 'day', 'sort' => 'asc'),
    array('data' => t('Pages'), 'field' => 'count'),
  );

  $results = visitors_days_of_month_data($header);
  $rows    = array();
  $i       = 0;
  $count   = 0;

  foreach ($results as $data) {
    $rows[] = array(
      ++$i,
      (int) $data->day,
      $data->count
    );

    $count += $data->count;
  }
  $output  = visitors_date_filter();

  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}


