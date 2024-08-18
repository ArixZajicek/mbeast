// Thanks to yocto (https://github.com/will-hut/Prototypical) for
// showing how to do sockets in processing!!

import java.net.*;
import java.util.Arrays;

import java.io.IOException;
import java.net.StandardProtocolFamily;
import java.net.UnixDomainSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.nio.file.Path;
import java.nio.*;

public class Socket
{
  byte[] packetData;
  SocketChannel channel;
  ByteBuffer buffer;
  boolean connected;
  
  Socket(PApplet parent) {
    buffer = ByteBuffer.allocateDirect(65536);
    parent.registerMethod("draw", this);
    this.connected = false;
  }
  
  boolean connect() {
    
    try {
      UnixDomainSocketAddress socketAddress = UnixDomainSocketAddress.of(SOCKET_PATH); // create socketAddress
      channel = SocketChannel.open(StandardProtocolFamily.UNIX); // create the channel
      channel.connect(socketAddress); // connect channel to address;
    } catch(IOException ex) {
      ex.printStackTrace();
      System.out.println("Could not connect to socket file. Is the server running?");
      return false;
    }
    
    this.connected = true;
    return true;
  }
  
  void draw() {
    if(this.connected){
      loadPixels();
      buffer.clear();
      // Command
      buffer.put((byte) 0x20);
      for (int i = 0; i < WIDTH * HEIGHT; i++) {
        int p = pixels[i];
        buffer.put((byte)(p >> 16));  //R
        buffer.put((byte)(p >> 8));   //G
        buffer.put((byte) p);         //B
      }
      
      buffer.flip();
      
      try{
        channel.write(buffer);
      } catch(IOException ex) {
        ex.printStackTrace();
        System.out.println("Could not write to socket file. Did the server crash?");
        this.connected = false;
      }
    }
  }
}
