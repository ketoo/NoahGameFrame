#1 pull the Dockerfile from github

rm -rf NoahGameFrame
git clone https://github.com/ketoo/NoahGameFrame.git

#2 building the Dockerfile to build a docker image with the app's binary file

var=`date "+%Y%m%d_%H%M"`
docker build -t var .

#3 push the image to docker repos

docker push jameshuang4nz/var

rm -rf NoahGameFrame

#4 the other server pull the docker image and run it

