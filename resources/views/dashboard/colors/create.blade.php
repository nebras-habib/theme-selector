<x-app-layout>
    <x-slot name="header">
        <h2 class="font-semibold text-xl leading-tight">
            {{ __('Colors') }}
        </h2>
    </x-slot>

    <div class="py-12">
        <div class="max-w-7xl mx-auto sm:px-6 lg:px-8">
            <div class="bg-white overflow-hidden shadow-sm sm:rounded-lg">
                <div class="p-6 bg-white border-b border-gray-200">

                    <!-- Validation Errors -->
                    <x-auth-validation-errors class="mb-4" :errors="$errors" />

                    <x-session-success-messages class="mb-4"/>

                    <form method="POST" action="{{ route('dashboard.colors.store') }}">
                        @csrf
                        <div class="mt-4">
                            <label for="primary_color" class="block font-medium text-sm text-gray-700">Primary Color</label>
                            <input id="primary_color" class="block mt-1 rounded-md shadow-sm border-gray-300" type="color" name="primary_color" value="{{$primaryColor}}" required />
                        </div>
                        <div class="mt-4">
                            <label for="secondary_color" class="block font-medium text-sm text-gray-700">Secondary Color</label>
                            <input id="secondary_color" class="block mt-1 rounded-md shadow-sm border-gray-300" type="color" name="secondary_color" value="{{$secondaryColor}}" required />
                        </div>
                        <div class="mt-4">
                            <input class="" type="checkbox" value="1" id="dark_mode" name="dark_mode" {{ $darkMode ? 'checked' : '' }}>
                            <label class="" for="status">
                            Enable Dark Mode
                            </label>
                        </div>
                        <div class="mt-4">
                            <button type="submit" class="inline-flex items-center px-4 py-2 bg-gray-800 border border-transparent rounded-md font-semibold text-xs text-white uppercase tracking-widest hover:bg-gray-700">
                                Update
                            </button>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</x-app-layout>