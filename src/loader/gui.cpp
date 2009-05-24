#include <smooth.h>
#include <smooth/main.h>
#include <smooth/args.h>

using namespace smooth;
using namespace smooth::System;

Int smooth::Main(const Array<String> &args)
{
#ifdef __WIN32__
	SetEnvironmentVariableA("PATH", "%PATH%;boca");
#endif

	DynamicLoader	*loader = new DynamicLoader("BonkEnc");
	Int		(*StartGUI)(const Array<String> &) = (Int (*)(const Array<String> &)) loader->GetFunctionAddress("StartGUI");
	Int		 result = StartGUI(args);

	Object::DeleteObject(loader);

	return result;
}
