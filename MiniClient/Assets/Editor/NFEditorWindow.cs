using UnityEngine;
using UnityEditor;
using System.Collections;

public static NFCServer CreateServer(string name) 
{
	GameObject go = new GameObject(name);
	NFCServer client = go.AddComponent<NFCServer>();
}
	
public static NFCClient CreateClient(string name) 
{

	GameObject go = new GameObject(name);
	NFCClient client = go.AddComponent<NFCClient>();
	
	return client;
	
}


[ExecuteInEditMode]
public class NFEditorWindow : EditorWindow {
	
	[MenuItem("Window/NF/New NF Server")]
    public static void CreateMultiServer() 
	{
		//NF.CreateServer();
	}
	
    [MenuItem("Window/NF/New NF Client")]
    public static void CreateCient() 
	{
		//NF.CreateClient();
	}


}
