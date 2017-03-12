import java.net.*;
import java.io.*;

public class TCPEchoServer{
  private static final int BUFSIZE = 32;

  public static void main(String [] args) throws IOException{
    if(args.length != 1)
    {throw new IllegalArgumentException("Parameter(s):<Port>");}

    int servPort = Integer.parseInt(args[0]);
    ServerSocket servSock = new ServerSocket(servPort);

    int recvMsgSize;
    byte[] receiveBuf = new byte[BUFSIZE];
    while (true)
    {
      Socket clntSock = servSock.accept();
      SocketAddress clientAddress = clntSock.getRemoteSocketAddress();
      InputStream in = clntSock.getInputStream();
      OutputStream out = clntSock.getOutputStream();

      while((recvMsgSize = in.read(receiveBuf)) != -1)
      {
        out.write(receiveBuf, 0, recvMsgSize);
      }
      clntSock.close();
    }
  }
}
