FROM ubuntu:16.04

MAINTAINER James.Huang <342006@qq.com>

RUN apt-get update \
&&  apt-get -y upgrade \
&& mkdir -p /mnt/data \
&& mkdir -p /mnt/data/apps \
&& mkdir -p /mnt/data/logs


RUN apt-get install -y git g++ cmake unzip autoconf automake subversion

ADD ./NoahGameFrame /mnt/data/apps/NoahGameFrame

RUN cd /mnt/data/apps/NoahGameFrame \
&& ./install4cmake.sh

#RUN cd /mnt/data/apps \
#&& wget redis.tar.gz "http://download.redis.io/releases/redis-3.2.5.tar.gz" \
#&& tar -xzf redis.tar.gz ./ \
#&& cd redis\
#&& make \
#&& cd ..
#&& rm redis.tar.gz

ENTRYPOINT ["/mnt/data/apps/NoahGameFrame/_Out/rund.sh"]