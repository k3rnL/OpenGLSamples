//
// Created by Erwan on 11-May-20.
//

#include "utils.h"

#define _AMD64_

#include <minwindef.h>
#include <libloaderapi.h>
#include <errhandlingapi.h>

std::ostream &operator<<(std::ostream & os, const glm::vec3 &v) {
    os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return os;
}

void initRenderDoc() {

// At init, on windows
    auto hinstLib = LoadLibrary(R"(renderdoc.dll)");
    if(HMODULE mod = GetModuleHandleA(R"(renderdoc.dll)"))
    {
        pRENDERDOC_GetAPI RENDERDOC_GetAPI =
                (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
        int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&rdoc_api);
        assert(ret == 1);
    } else
        std::cerr << "failed renderdoc" << GetLastError() << std::endl;

}
