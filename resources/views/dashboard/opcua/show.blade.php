<x-app-layout>
    <x-slot name="header">
        <h2 class="font-semibold text-xl leading-tight">
            {{ $pump }}
        </h2>
    </x-slot>

    <div class="py-12">
        <div class="max-w-7xl mx-auto sm:px-6 lg:px-8">
            <div class="bg-white overflow-hidden shadow-sm sm:rounded-lg">
                <div class="p-6 bg-white border-b border-gray-200">
                    <x-session-success-messages class="mb-4" />

                    @foreach($pumpData as $data)
                    <div class="relative">
                        <label class="">{{ $data["name"] }} : {{ $data["value"] }}</label>
                    </div>
                    @endforeach

                    @foreach($pumpMethods as $method)
                    <div>
                        <form method="POST" action="{{ route( 'dashboard.opcua.pump.callMethod' ,[$pump , $method['methodId'] ] ) }}">
                            @csrf
                            <div class="mt-4">
                                <button type="submit" class="inline-flex items-center px-4 py-2 bg-gray-800 border border-transparent rounded-md font-semibold text-xs text-white uppercase tracking-widest hover:bg-gray-700">
                                    {{ $method["name"] }}
                                </button>
                            </div>
                        </form>
                    </div>
                    @endforeach

                </div>
            </div>
        </div>
    </div>
</x-app-layout>