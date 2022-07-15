<?php

namespace App\Http\Controllers\Dashboard;

use App\Http\Controllers\Controller;
use Illuminate\Support\Facades\Auth;
use Illuminate\Http\Request;
use App\Models\Setting;

class ColorController extends Controller
{
    public function create()
    {
        $user = Auth::user();
        $primaryColor = $user->primary_color;
        $secondaryColor = $user->secondary_color ;
        $darkMode = $user->dark_mode;
        return view('dashboard.colors.create', compact('primaryColor', 'secondaryColor', 'darkMode'));
    }

    public function store(Request $request)
    {

        $validatedData = $request->validate([
            'primary_color' => ['required', 'regex:/^#[a-f0-9]{6}$/i'],
            'secondary_color' => ['required', 'regex:/^#[a-f0-9]{6}$/i'],
            'dark_mode' => ['nullable']
        ]);

        $userId = Auth::id();
        Setting::upsert(
            [
                [
                    'user_id' => $userId,
                    'key' => 'primary-color',
                    'value' => $validatedData['primary_color']
                ],
                [
                    'user_id' => $userId,
                    'key' => 'secondary-color',
                    'value' => $validatedData['secondary_color']
                ]
            ],
            [
                'user_id', 'key'
            ],
            ['value']
        );

        $darkMode  = $request->has('dark_mode') ? true : false;
        Setting::updateOrCreate(
            ['user_id' => $userId, 'key' => 'dark-mode'],
            ['value' => $darkMode]
        );

        session()->flash('success', 'Colors Updated Successfully');
        return redirect()->back();
    }
}
