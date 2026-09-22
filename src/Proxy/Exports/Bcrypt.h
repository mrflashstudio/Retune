#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptAddContextFunction=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptAddContextFunction,@1")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptAddContextFunctionProvider=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptAddContextFunctionProvider,@2")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptCloseAlgorithmProvider=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptCloseAlgorithmProvider,@3")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptConfigureContext=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptConfigureContext,@4")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptConfigureContextFunction=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptConfigureContextFunction,@5")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptCreateContext=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptCreateContext,@6")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptCreateHash=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptCreateHash,@7")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptCreateMultiHash=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptCreateMultiHash,@8")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptDecapsulate=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptDecapsulate,@9")
#pragma comment(                                                                                             \
	linker, "/EXPORT:BCryptDecrypt=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptDecrypt,@10")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptDeleteContext=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptDeleteContext,@11")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptDeriveKey=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptDeriveKey,@12")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptDeriveKeyCapi=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptDeriveKeyCapi,@13")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptDeriveKeyPBKDF2=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptDeriveKeyPBKDF2,@14")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptDestroyHash=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptDestroyHash,@15")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptDestroyKey=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptDestroyKey,@16")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptDestroySecret=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptDestroySecret,@17")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptDuplicateHash=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptDuplicateHash,@18")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptDuplicateKey=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptDuplicateKey,@19")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptEncapsulate=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptEncapsulate,@20")
#pragma comment(                                                                                             \
	linker, "/EXPORT:BCryptEncrypt=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptEncrypt,@21")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptEnumAlgorithms=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptEnumAlgorithms,@22")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptEnumContextFunctionProviders=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptEnumContextFunctionProviders,@23")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptEnumContextFunctions=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptEnumContextFunctions,@24")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptEnumContexts=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptEnumContexts,@25")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptEnumProviders=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptEnumProviders,@26")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptEnumRegisteredProviders=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptEnumRegisteredProviders,@27")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptExportKey=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptExportKey,@28")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptFinalizeKeyPair=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptFinalizeKeyPair,@29")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptFinishHash=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptFinishHash,@30")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptFreeBuffer=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptFreeBuffer,@31")
#pragma comment(linker, "/EXPORT:BCryptGenRandom=ProxyBCryptGenRandom,@32")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptGenerateKeyPair=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptGenerateKeyPair,@33")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptGenerateSymmetricKey=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptGenerateSymmetricKey,@34")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptGetFipsAlgorithmMode=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptGetFipsAlgorithmMode,@35")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptGetProperty=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptGetProperty,@36")
#pragma comment(                                                                                             \
	linker, "/EXPORT:BCryptHash=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptHash,@37")
#pragma comment(                                                                                             \
	linker, "/EXPORT:BCryptHashData=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptHashData,@38")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptImportKey=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptImportKey,@39")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptImportKeyPair=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptImportKeyPair,@40")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptKeyDerivation=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptKeyDerivation,@41")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptOpenAlgorithmProvider=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptOpenAlgorithmProvider,@42")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptProcessMultiOperations=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptProcessMultiOperations,@43")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptQueryContextConfiguration=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptQueryContextConfiguration,@44")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptQueryContextFunctionConfiguration=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptQueryContextFunctionConfiguration,@45")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptQueryContextFunctionProperty=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptQueryContextFunctionProperty,@46")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptQueryProviderRegistration=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptQueryProviderRegistration,@47")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptRegisterConfigChangeNotify=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptRegisterConfigChangeNotify,@48")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptRegisterProvider=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptRegisterProvider,@49")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptRemoveContextFunction=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptRemoveContextFunction,@50")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptRemoveContextFunctionProvider=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptRemoveContextFunctionProvider,@51")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptResolveProviders=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptResolveProviders,@52")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptSecretAgreement=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptSecretAgreement,@53")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptSetAuditingInterface=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptSetAuditingInterface,@54")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptSetContextFunctionProperty=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptSetContextFunctionProperty,@55")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptSetProperty=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptSetProperty,@56")
#pragma comment(                                                                                             \
	linker, "/EXPORT:BCryptSignHash=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptSignHash,@57")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptUnregisterConfigChangeNotify=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptUnregisterConfigChangeNotify,@58")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptUnregisterProvider=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptUnregisterProvider,@59")
#pragma comment(linker,                                                                                      \
	"/EXPORT:BCryptVerifySignature=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\bcrypt.dll.BCryptVerifySignature,@60")
