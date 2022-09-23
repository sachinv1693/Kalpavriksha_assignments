== Protocol format = 

All client server communication will share the information in following format

TDSProtocolVersion: 1.0
TDSProtocolFormat: json
{
	"type" : "type",
	"header": { 
			"key1": "value1",
			"key2": "value2",
			"key3": "value3"
	}
	"payload" : base64string
}


Equivalent Protocol definition in XML:
TDSProtocolVersion: 1.0
TDSProtocolFormat: xml
<protocol>
	<type>value</type>
	<header>
		<header-key1>value</header-key1>
		<header-key2>value</header-key2>
		<header-key3>value</header-key3>
	</header>
	
</protocol>

The above json/xml represetnation must be DeSerialized to a corresponding object as follows

Protocol Object definiton, define getter/setter for all the attributes:
class TDSProtocol{
	private String sourceIP;
	private String destIp;
	private int sourcePort;
	private int destPort;
	//Defines the version of protocol and maps to "TDSProtocolVersion" attribute in the protocol
	private String protocolVersion;
	//Defines the format of protocol and maps to "TDSProtocolFormat" attribute in the protocol
	private String protocolFormat;
	//Corresponds to "type" attribute in protocol definition, defines the type of request, could be a request
	// or a response type.
	private String protocolType;
	//Contains the key value pairs as specified in protocol "header" object.
	private HashMap<String,String> headers;

	//public String getHeader(String headerKey);
	//public String setHeader(String headerKey,String value);
}	


Example Request and Response representatio in JSON using the TDSProtocol format to perform add node operation.

Example Request

	
{
	"type" : "request",
	"header": {
		"TDSProtocolVersion": "1.0",
		"TDSProtocolFormat": "json"
	},
	"payload" : {
		"method": "node-add",
		"node-name": "abc",
		"node-ip": "ip-address",
		"node-port": "port"
	}
}


Example Response:
TDSProtocolVersion: 1.0
TDSProtocolFormat: json
{
	"type" : "response",
	"header": { 
			"status": "SUCCESS",
			"node-id": "121",
			"error-code": "200",
			"error-message": "message"
	}
}

Create two classes each representing Request and a Response both inheriting from TDSProtocol.

class TDSRequest extends TDSProtocol{
	//corresponds to method in the header
	public String getMethod();
	public String setMethod(String method);

	//Get's the value of a given parameter with specified key
	public String getParameter(String key);

	//Adds a parameter for the request
	public void addParameter(String key, String value);
}

class TDSResponse extends TDSProtocol{
	//Should return true/false, corresponds to status tag in the response
	public boolean getStatus();
	public int getErrorCode();
	public int getErrorMessage();

	//Should return the value for a specific key from the response data, for example to retrieve the node-id
	public String getValue(String key);
	public void setValue(String key,String value);

}

+ Serializable interface

Implement following interface to Serialize/DeSerialize the json/xml into the corresponding Objects.

interface TDSSerializer{
	//DeSerialize the String data into a TDSProtocol object
	TDSProtocol DeSerialize(String data);

	//Serialize the TDSProtocol object to a String format
	String Serialize(TDSProtocol protocol); 	
}

Implement this interface as a JSONSerializer or XMLSeriaizer and also implement a Factory class to get the Serializer object.

+ Server implementation

main() {
	TDSServer server = new TDSServer();
	server.start();
}
TDSServer.java
------------
class TDSServer{
	//Initialize a ServerSocket in constructor on a given port.
	//Define the port configuration in TDSConfiguration and use that instead of hardcoding.
	public TDSServer(){}

	public void start(){
	//Do the following sequence in a while loop.
		//Wait for data to arrive at the socket.
		//Read the data
		//De-Serialize to get the Request.
		//Log the request
		//process request
		//Construct a TDSResponse
		//Serialize the TDSResponse
		//Write the serialized output to the socket
	}
	
}
+ Client implementation.

main() {

TDSRequest request=new TDSRequest();
request.setMethod("add-node");
request.addParameter("node-name","node1");
request.addParamter("node-ip","Ip-address");
request.addParamter("node-port","port");

TDSClient client = new TDSClient();
TDSResponse response = client.sendRequest(request);

System.out.println("Resposne status:"+response.getStatus());
System.out.println("Response error code:"+response.getErrorCode());
System.out.println("Response error message:"+response.getErrorMessage());


}
	

TDSClient.java
----------

class Client{
   private readonly ITDSSerializer _serializer;
   private SerializationFactory _factory;
   public Client()
   {
		_factory = new SerializationFactory();
        _serializer = _factory.GetSerializer(SerializerType.JSON);
   }
   async Task<TDSResponse> sendRequest(TDSRequest request){
      //Open a socket connection to server
      //Serialize request object
	  string serializedData = _serializer.Serialize(request);
	  
	  //Send the serialized data to server and wait for response
	  string data = await SendOverSocket(serializedData);
      //De-Serialize the response and return
	  TDSResponse response = _serializer.De-Serialize(data);
	  return response;
   }


+ Advanced Server implementation(NOT TO BE IMPLEMENTED FOR NOW)

main() {
	//Here you are in a loop to recieve the request
	while(1){
		//Assume you recieved a request and the request data is in a variable say requestData
		String response = processRequest(requestData);

		//Write the response to the socket

	}
}

String processRequest(String requestData){

TDSSerializer dataSerializer=  TDSSerializerFactory.getSerializer();

TDSRequest request = (TDSRequest) dataSerializer.DeSerialize(requestData);

System.out.println("Request method:"+request.getMethod());
TDSResponse response = new TDSResponse();
response.setStatus(true);
response.setErrorCode(0);
response.setErrroMessage("Operation complted succcessfully");
response.setValue("node-id","123");

String response = dataSerializer.Serialize(response);

return response;
}


====Multi Threaded request dispatcher ==

Objective: Make the server multi-threaded and have a separate dispatcher to identify a controller to handle the requests.


//Executes requests in a thread
class SocketHandler implements Runnable{
	Socket sock;
	SocketHandler(Socket sock){
		this.sock = sock;
	}
	void run(){
		String requestData = getRequest(sock);
		//Convert the request to TDSRequest 
		TDSController controller= RequestDispatcher.getController(TDSRequest request);
		TDSResponse response = controller.processRequest(request);	
TDSResponse response = new TDSResponse();
response.setStatus(true);
response.setErrorCode(0);
response.setErrroMessage("Operation		String responseData = //Serialize the response data;
		writeResponse(sock,responseData);
	}
	private String getRequest(Socket sock);
	private void writeResponse(Socket sock,responseData);
}
The RequestDispatcher should dispatch the request to appropriate controller. Instead of having method name as add-node, I recommend that
we call it as follows so that identification of the dispatcher is easier.
node-add
node-modify
node-delete

class RequestDispatcher{
	public static TDSController getController(TDSRequest request){
		if(request.getMethod().startsWith("node-")){
			NodeController nodeController = new NodeController();
			return nodeController;
		}	
	}
}

2)Implement NodeController that will handle the Node related requests

interface TDSController{
	public TDSResponse processRequest(TDSRequest request);	
}

class NodeController implements TDSController{
	private static final String ADD_REQ="node-add";
	TDSResponse processRequest(TDSRequest request){
		if(request.getMethod().equals(ADD_REQ)){
			return	AddNode(request);
		}
		
	}
	public TDSResponse AddNode(TDSRequest request){
	//TODO
	}
}

Client-Server-Node communication





