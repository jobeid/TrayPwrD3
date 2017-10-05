#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"

// this will only call release if an object exists (prevents exceptions calling release on non existant objects)
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }
#define FRAME_BUFFER_COUNT_X  2 // number of buffers we want, 2 for double buffering, 3 for tripple buffering

class D3dx12jo 
{
public:
	D3dx12jo();

	// function declarations
	bool InitD3D(HWND hwnd); // initializes direct3d 12
	void Render(); // execute the command list
	void Cleanup(); // release com ojects and clean up memory
	void CloseFenceHandle(); //CloseHandle(fenceEvent);

protected:
	void UpdatePipeline(); // update the direct3d pipeline (update command lists)
	void WaitForPreviousFrame(); // wait until gpu is finished with command list
								 // Handle to the window
	HWND hwnd_jo;
	// is window full screen?
	bool FullScreen_jo;

	// we will exit the program when this becomes false
	bool Running;
	// direct3d stuff

	ID3D12Device* device; // direct3d device

	IDXGISwapChain3* swapChain; // swapchain used to switch between render targets

	ID3D12CommandQueue* commandQueue; // container for command lists

	ID3D12DescriptorHeap* rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets

	ID3D12Resource* renderTargets[FRAME_BUFFER_COUNT_X]; // number of render targets equal to buffer count

	ID3D12CommandAllocator* commandAllocator[FRAME_BUFFER_COUNT_X]; // we want enough allocators for each buffer * number of threads (we only have one thread)

	ID3D12GraphicsCommandList* commandList; // a command list we can record commands into, then execute them to render the frame

	ID3D12Fence* fence[FRAME_BUFFER_COUNT_X];    // an object that is locked while our command list is being executed by the gpu. We need as many 
											 //as we have allocators (more if we want to know when the gpu is finished with an asset)

	HANDLE fenceEvent; // a handle to an event when our fence is unlocked by the gpu

	UINT64 fenceValue[FRAME_BUFFER_COUNT_X]; // this value is incremented each frame. each fence will have its own value

	int frameIndex; // current rtv we are on

	int rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)
};

