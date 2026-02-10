for i in client broker worker
do
	cd $i; docker build -t im$i .; cd ..
done
