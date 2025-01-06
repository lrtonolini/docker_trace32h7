FROM lscr.io/linuxserver/webtop:ubuntu-openbox AS extractor

ARG T32

RUN apt-get update \
  && apt-get install --no-install-recommends -y \
  && rm -rf /var/lib/apt/lists/*

COPY ${T32} /opt/
RUN chmod +x /opt/Trace32_H7/TRACE32/bin/pc_linux64/t32marm 

FROM lscr.io/linuxserver/webtop:ubuntu-openbox

RUN apt-get update \
  && apt-get install --no-install-recommends -y \
      libcurl4 \
      libqt5core5a \
      libqt5gui5 \
      libqt5printsupport5 \
      libqt5svg5 \
      libqt5widgets5 \
      libuuid1 \
      zlib1g \
      gedit \
  && rm -rf /var/lib/apt/lists/*

# copy TRACE32
COPY --from=extractor /opt/Trace32_H7 /opt/Trace32_H7

RUN chmod +x /opt/Trace32_H7/Trace32_H7_linux.sh \
  && chmod +x /opt/Trace32_H7/TRACE32/bin/pc_linux64/udev.conf/setup_udevrules.sh


# Intercom
EXPOSE 10000/udp
# RemoteAPI (TCP)
EXPOSE 20000/tcp

WORKDIR /opt/Trace32_H7





