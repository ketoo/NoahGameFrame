#1 pull the Dockerfile from github

#2 building the Dockerfile to build a docker image with the app's binary file

docker build -t date +%Y%m%d_%H%M .

#3 push the image to docker repos

docker push jameshuang4nz/sss

#4 the other server pull the docker image and run it

