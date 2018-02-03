using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;   
using UnityEngine.UI;
using System;


public class main1 : MonoBehaviour {


	[DllImport ("openCV_0112_1")]      
	private static extern void showUnity3dCamera(IntPtr data,int width,int height,int stride,CSCallback cs);



	public WebCamTexture webTexture;
	public RawImage rawImage;


	public Transform sp;

	float timer=1.0f;




	public delegate void CSCallback(int x,int y);  
    CSCallback callback;  


	public Vector3 screenPos;

	void Start () {  
		callback = CSCallbackFuction;  
	}  

	public int X1, Y1;
    void CSCallbackFuction(int x,int y)  
	{  
		//Debug.Log ("X:"+x+" Y:"+y);  
		X1 = x;
		Y1 = y;
	}  

	private void Trans(){
		
		Vector3 v = Camera.main.ScreenToWorldPoint (new Vector3(X1,Y1,2));
		sp.transform.position = v;

	}

	void Update(){



		Trans ();






		timer += Time.deltaTime;
		if (timer > 0.1f) {
			timer = 0.0f;
			SendImageToCPP ();
		}

	}


	public void SendImageToCPP(){
		if (webTexture != null) {
			Color32[] pixels = webTexture.GetPixels32 ();
			GCHandle pixelHandle = GCHandle.Alloc(pixels, GCHandleType.Pinned);
			IntPtr pixelPointer = pixelHandle.AddrOfPinnedObject();
			showUnity3dCamera(pixelPointer,webTexture.width,webTexture.height,4,callback);
		}
	}


	public void OnClickCamera(){
		StartCoroutine (StartCamera());
	}


	private IEnumerator StartCamera(){

		yield return Application.RequestUserAuthorization (UserAuthorization.WebCam);


		if (Application.HasUserAuthorization (UserAuthorization.WebCam)) {

			WebCamDevice[] devices = WebCamTexture.devices;
			string deviceName="";
			if (devices.Length > 0) {
				deviceName=devices[0].name;
			}

			webTexture = new WebCamTexture (deviceName);
			rawImage.texture = webTexture;
			webTexture.Play ();

		}
	}

}
