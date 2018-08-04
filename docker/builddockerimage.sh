export DOCKER_ID_USER="jameshuang4nz" #please use your account
export DOCKER_ID_PWD="1234567a"#please use your password
export REPOS_NAME="noahframe"

#1 pull the Dockerfile from github

rm -rf NoahGameFrame
git clone -b develop https://github.com/ketoo/NoahGameFrame.git

#2 building the Dockerfile to build a docker image with the app's binary file

var=`date "+%Y%m%d_%H%M"`
docker build -t $DOCKER_ID_USER/$REPOS_NAME:$var .

#3 push the image to docker repos

docker login -u $DOCKER_ID_USER -p $DOCKER_ID_PWD
docker push $DOCKER_ID_USER/$REPOS_NAME:$var

rm -rf NoahGameFrame

#4 the other server pull the docker image and run it

#docker pull --all-tags jameshuang4nz/noahframe