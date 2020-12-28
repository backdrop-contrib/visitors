<?php

/**
 * @file
 * Monthly history report for the visitors module.
 */

/**
 * Display monthly history report.
 *
 * @return
 *   string monthly history report html source.
 */
function visitors_monthly_history() {
  $items_per_page = config_get('visitors.settings', 'visitors_lines_per_page');

  $header = array(
    array('data' => t('#')),
    array('data' => t('Month'), 'field' => 'm', 'sort' => 'asc'),
    array('data' => t('Pages'), 'field' => 'count'),
  );

  $query = db_select('visitors', 'v')
    ->extend('PagerDefault')
    ->extend('TableSort');
  $query->addExpression('COUNT(*)', 'count');
  $query->addExpression(
    visitors_date_format_sql('visitors_date_time', '%Y%m'), 'm'
  );
  $query->addExpression(
    visitors_date_format_sql('MIN(visitors_date_time)', '%Y %M'), 's'
  );
  visitors_date_filter_sql_condition($query);
  $query->groupBy('m');
  $query->orderByHeader($header);
  $query->limit($items_per_page);

  $count_query = db_select('visitors', 'v');
  $count_query->addExpression(
    sprintf('COUNT(DISTINCT %s)',
    visitors_date_format_sql('visitors_date_time', '%Y %M'))
  );
  visitors_date_filter_sql_condition($count_query);
  $query->setCountQuery($count_query);
  $results = $query->execute();

  $rows = array();
  $i = 1;
  foreach ($results as $data) {
    $rows[] = array(
      $i,
      $data->s,
      $data->count
    );
    $i++;
  }

  $output  = visitors_date_filter();

  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= theme('pager', array('quantity' => $items_per_page));

  return $output;
}

