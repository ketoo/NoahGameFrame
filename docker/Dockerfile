FROM ubuntu:16.04

MAINTAINER James.Huang <342006@qq.com>

RUN apt-get update \
&&  apt-get -y upgrade


RUN apt-get install -y git g++ cmake unzip autoconf automake subversion

# create working directory
RUN mkdir -p /mnt/data \
&& mkdir -p /mnt/data/apps \
&& mkdir -p /mnt/data/logs \
&& cd /mnt/data/apps \
&& git clone https://github.com/ketoo/NoahGameFrame.git \
&& cd ./NoahGameFrame \
&& ./install4cmake.sh


RUN cd /mnt/data/apps \
&& wget redis.tar.gz "http://download.redis.io/releases/redis-3.2.5.tar.gz" \
&& tar -xzf redis.tar.gz ./ \
&& cd redis\
&& make \
&& rm -rf /var/lib/apt/lists/* \
&& rm redis.tar.gz


#ENTRYPOINT ["/mnt/data/apps/NoahGameFrame/_Out/rund.sh"]