SimHash PHP Extension
=======

SimHash(相似性哈希)可以计算两个实体之间的相似程度，用于识别抄袭。更多信息见 [SimHash or the way to compare quickly two datasets](http://titouangalopin.com/blog/2013/11/simhash-or-the-way-to-compare-quickly-two-datasets)。

php7
-----

增加了新的分支，支持php7

    git checkout php7

Install
-----

	phpize
	./configure
	make
	make install

API
----

	string genhash(array $dataset, $mixed $weights)

生成数据集合`dataset`的hash, `weights`是每个数据项的权重（比如词的`TF/IDF`），如果`weights`为`NULL`，则所有数据项的权重按照`1`计算(等概率)。


	double cmphash(string hash1, string hash2)
	
比较两个生成的hash值，返回两个hash中相同位的百分比。`0`表示完全不同，`1`表示完全相同。

Example
-----

	function simhash($d1, $d2) {
    	var_dump(
        	cmphash(
            	genhash(split(' ', $d1), NULL),
            	genhash(split(' ', $d2), NULL)
        	)
    	);
	}
	
	$a1 = 'The only formal pre-requisites was to have successfully completed “Intro to Data Science” or some basic course on machine learning. Familiarity with computer programming is assumed. The course relies heavily on such a mathematical tools as the linear algebra, probability and statistics method, multi-variate calculus, and function optimization. The basic mathematical concepts will be introduced when we needed, but students will be expected to assimilate a non-trivial amount of mathematical concepts in a fairly short time. haha';
	$a2 = 'The only formal pre-requisites is to have successfully completed “Intro to Data Science” or any basic course on machine learning. Familiarity with computer programming is assumed. The course relies heavily on such mathematical tools as linear algebra, probability and statistics, multi-variate calculus, and function optimization. The basic mathematical concepts will be introduced when needed, but students will be expected to assimilate a non-trivial amount of mathematical concepts in a fairly short time.';
	simhash($a1, $a2); //output 0.88125
