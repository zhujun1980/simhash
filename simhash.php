<?php
//var_dump(genhash(array(2, 1, 2, 4, 5), array(1, 2, 3, 4, 5)));
//var_dump(genhash(array(2, 1, 2, 4, 5), NULL));
//var_dump(genhash(array(2, 1, 2, 4, 5), array(1, NULL, false, "abc", array())));
//var_dump(genhash(array(), array()));

var_dump(
    cmphash(
        genhash(array(2, 1, 2, 4, 5), NULL),
        genhash(array(999, 12122, 333, 111), NULL)
        //genhash(array(2, 1, 3, 4, 5), NULL)
    )
);

$a1 = 'The only formal pre-requisites was to have successfully completed “Intro to Data Science” or some basic course on machine learning. Familiarity with computer programming is assumed. The course relies heavily on such a mathematical tools as the linear algebra, probability and statistics method, multi-variate calculus, and function optimization. The basic mathematical concepts will be introduced when we needed, but students will be expected to assimilate a non-trivial amount of mathematical concepts in a fairly short time. haha';
$a2 = 'The only formal pre-requisites is to have successfully completed “Intro to Data Science” or any basic course on machine learning. Familiarity with computer programming is assumed. The course relies heavily on such mathematical tools as linear algebra, probability and statistics, multi-variate calculus, and function optimization. The basic mathematical concepts will be introduced when needed, but students will be expected to assimilate a non-trivial amount of mathematical concepts in a fairly short time.';
simhash($a1, $a2);

simhash("John is in high school. He is not so tall.", "Mary-jane is very tall. She was in the 9th grade."); // < 0.8
simhash("Mary-jane is very tall. She was in the 9th grade.", "Mary-jane is very tall. She is in the 9th grade."); // >0.9

simhash("Mary-jane is very tall. She was in the 9th grade.", "已知 两篇 文档 Simhash 值 之间 的 海明距离 越小"); // >0.9
simhash("已知 两篇 文章 SimHash 值 之间 的 海明距离 越小", "已知 两篇 文档 Simhash 值 之间 的 海明距离 越小"); // >0.9

function simhash($d1, $d2) {
    var_dump(
        cmphash(
            genhash(split(' ', $d1), NULL),
            genhash(split(' ', $d2), NULL)
        )
    );
}
