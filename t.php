<?php
function simhash($d1, $d2) {
    //echo count(explode(' ', $d1));
    //print_r (range(0, count(explode(' ', $d1))));
    $h1 = genhash(explode(' ', $d1), range(0, count(explode(' ', $d1)) - 1));
    //$h1 = genhash(explode(' ', $d1), NULL);
    $h2 = genhash(explode(' ', $d2), NULL);
    //echo md5($h1);
    var_dump(cmphash($h1, $h2));
}

$a1 = 'The only formal pre-requisites was to have successfully completed “Intro to Data Science” or some basic course on machine learning. Familiarity with computer programming is assumed. The course relies heavily on such a mathematical tools as the linear algebra, probability and statistics method, multi-variate calculus, and function optimization. The basic mathematical concepts will be introduced when we needed, but students will be expected to assimilate a non-trivial amount of mathematical concepts in a fairly short time. haha';
$a2 = 'The only formal pre-requisites is to have successfully completed “Intro to Data Science” or any basic course on machine learning. Familiarity with computer programming is assumed. The course relies heavily on such mathematical tools as linear algebra, probability and statistics, multi-variate calculus, and function optimization. The basic mathematical concepts will be introduced when needed, but students will be expected to assimilate a non-trivial amount of mathematical concepts in a fairly short time.';

simhash($a1, $a2); //output 0.88125

