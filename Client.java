import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class Client {

    public static void main(String[] args) {
        String hostName = "127.0.0.1";
        int portNumber = 8080;

        try (Socket socket = new Socket(hostName, portNumber);
             PrintWriter out = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()), true);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));) {

            BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

            //Input name
            System.out.print("Enter your name: ");
            String name = stdIn.readLine();

            // Start thread to receive data from the server
            Thread receiveThread = new Thread(() -> {
                try {
                    String response;
                    while ((response = in.readLine()) != null) {
                        System.out.println(response);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            });
            receiveThread.start();

            // Start thread to send data to the server
            Thread sendThread = new Thread(() -> {
                try {
                    String userInput;
                    while ((userInput = stdIn.readLine()) != null) {
                        out.println("@" + name + ":" + userInput);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            });
            sendThread.start();

            // Wait for threads to finish
            receiveThread.join();
            sendThread.join();

        } catch (UnknownHostException e) {
            System.err.println("Don't know about host " + hostName);
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to " + hostName);
            System.exit(1);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

}
