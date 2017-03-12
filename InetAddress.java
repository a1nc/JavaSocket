import java.util.*;
import java.net.*;

public class InetAddress{
  public static void main(String [] args)
  {
    try{
      Enumeration<NetworkInterface> interfaceList = NetworkInterface.getNetworkInterfaces();
      //获取主机网络接口列表
      if(interfaceList==null)
      {
        System.out.println("==No interface found==");
      }
      else
      {
        while(interfaceList.hasMoreElements())
        {
          NetworkInterface iface=interfaceList.nextElement();
          System.out.println("Interface "+iface.getName()+":");
          //getName()方法为接口返回一个本地名称
          Enumeration<InetAddress> addrList=iface.getInetAddresses();
          if(!addrList.hasMoreElements())
          {
            System.out.println("\t(No address for this interface)");
          }
          while(addrList.hasMoreElements()) {
            InetAddress address = addrList.nextElement();
            if (address instanceof Inet4Address)
            {
              System.out.print("\tAddress(v4):");
            }
            else if(address instanceof Inet6Address)
            {
              System.out.print("\tAddress(v6):");
            }
            System.out.println(":"+address.getHostAddress());
          }
        }
      }
    }catch (SocketException se)
    {
      System.out.println("Error getting network interfaces:"+se.getMessage());
      //对getNetworkInterfaces()方法的调用将会抛出SocketException异常
    }
    for(String host:args)
    {
      try{
        System.out.println(host+":");
        InetAddress[] addressList=InetAddress.getAllByName(host);
        for(InetAddress address:addressList)
        {
          System.out.println("\t"+address.getHostName()+"/"+address.getHostAddress());
        }
      }catch (UnknownHostException e)
      {
        System.out.println("\tUnable to find address for "+host);
      }
    }
  }
}
