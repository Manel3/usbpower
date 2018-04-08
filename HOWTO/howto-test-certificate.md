## Creating Test Certificate

Windows 10 requires drivers to be digitally signed. It is possible to use a Test Certificate during developement and testing.

You must get some required utilities, read my [howto-tools.md](https://github.com/Manel3/usbpower/blob/master/HOWTO/howto-tools.md)
- Signtool.exe (Windows SDK) to sign the .cat 

### Signing the driver (the .inf file)
For CDFilter driver in dir \DEVEL\WDF-I) after compile solution with VS 2017 Community + WDK Windows Driver Kit:
- (Inf2Cat **Not-needed and Not-working**)
- makecert -r -pe -ss PrivateCertStore -n CN=Contoso.com(Test) ContosoTest.cer
- cd \DEVEL\WDF-I\CDFilter\x64\Debug\CDFilter
- signtool sign /v /s PrivateCertStore /n Contoso.com(Test) /t http://timestamp.verisign.com/scripts/timstamp.dll cdfilter.cat

This signed driver with Test Certificate can be installed in another PC without installing any public key on it.
 NOTE: it looks like secure boot must be disabled.

### (Not-needed) To install the self-signed certificate in another machine:
- [Export X.509 certificate](http://www.itprotoday.com/management-mobility/q-how-can-i-export-x509-certificate-and-associated-private-key-my-old-laptop-and):
	+ run **certmgr.msc**, select PrivateCertStore > Certificates, select certificate, right-click > All-Tasks > Export
	+ mark **Do not export the Private Key**
	+ select X.509 base 64 format (.CER) - (when exported the private key: select format PKCS#12 (.PFX) and set a password)
- Install the certificate by double-click on it

### Links
- Reading: http://www.davidegrayson.com/signing/
- [Questions and Answers: Windows 10 Driver Signing](https://www.osr.com/blog/2015/07/24/questions-answers-windows-10-driver-signing/)
- [Creating Test Certificates](https://docs.microsoft.com/en-us/windows-hardware/drivers/install/creating-test-certificates)
- [Test Certificates](https://docs.microsoft.com/en-us/windows-hardware/drivers/install/test-certificates)
- [Catalog Files and Digital Signatures](https://docs.microsoft.com/en-us/windows-hardware/drivers/install/catalog-files)
- [Test-Signing a Catalog File](https://docs.microsoft.com/en-us/windows-hardware/drivers/install/test-signing-a-catalog-file)

### TODO
**MakeCert is deprecated**. To create self-signed certificates, you can use the Powershell Cmdlet New-SelfSignedCertificate.

- [Create a Self-Signed Certificate Using PowerShell](https://www.petri.com/create-self-signed-certificate-using-powershell)
- [help for New-SelfSignedCertificate](https://docs.microsoft.com/en-us/powershell/module/pkiclient/new-selfsignedcertificate?view=win10-ps)

#### Creating Test Certificate with MakeCert.exe
makecert -r -pe -ss PrivateCertStore -n CN=Contoso.com(Test) ContosoTest.cer

#### MakeCert.ps1 (TODO)
It is posible to create a certificate in powershell **(not tested)**
``` powershell
"Major PSVersion must be 5"
$PSVersionTable.PSVersion
$cert = New-SelfSignedCertificate -certstorelocation cert:\localmachine\my -dnsname testcert.petri.com
$pwd = ConvertTo-SecureString -String ‘passw0rd!’ -Force -AsPlainText
$path = 'cert:\localMachine\my\' + $cert.thumbprint Export-PfxCertificate -cert $path -FilePath c:\temp\cert.pfx -Password $pwd
```
