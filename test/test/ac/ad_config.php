<?php
class Config{ public static function set($n, $v){var_dump($n, $v);}}
Config::set(
	'ad_info', array(
		'ad_target'=>array(
			'waku0'=>array(),
			'waku1'=>array('code0'),
			'waku2'=>array('code0', 'code1'),
		),
		'ad_data'=>array(
			'code0'=>array(
				'target'=>'tg0',
				'bg'=>'/bg/0',
				'titleimg'=>'/img/0',
				'comment'=>'comment 日本語0',
				'attention'=>'ATT0',
				'link'=>'https://ggmoyou.com/0',
			),
			'code1'=>array(
				'target'=>'tg1',
				'bg'=>'/bg/1',
				'titleimg'=>'/img/1',
				'comment'=>'comment 日本語1',
				'attention'=>'ATT1',
				'link'=>'https://ggmoyou.com/1',
			),
			'code2'=>array(
				'target'=>'tg2',
				'bg'=>'/bg/2',
				'titleimg'=>'/img/2',
				'comment'=>'comment 日本語2',
				'attention'=>'ATT2',
				'link'=>'https://ggmoyou.com/2',
			),
			'code3'=>array(
				'target'=>'tg3',
				'bg'=>'/bg/3',
				'titleimg'=>'/img/3',
				'comment'=>'comment 日本語3',
				'attention'=>'ATT3',
				'link'=>'https://ggmoyou.com/3',
			),
			'code4'=>array(
				'target'=>'tg4',
				'bg'=>'/bg/4',
				'titleimg'=>'/img/4',
				'comment'=>'comment 日本語4',
				'attention'=>'ATT4',
				'link'=>'https://ggmoyou.com/4',
			),
		)
	)
);
