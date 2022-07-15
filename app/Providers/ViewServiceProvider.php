<?php

namespace App\Providers;

use Illuminate\Support\ServiceProvider;
use Illuminate\Support\Facades\View;
use Illuminate\Support\Facades\Auth;

class ViewServiceProvider extends ServiceProvider
{
    /**
     * Register services.
     *
     * @return void
     */
    public function register()
    {
        //
    }

    /**
     * Bootstrap services.
     *
     * @return void
     */
    public function boot()
    {
        View::composer('layouts.app', function ($view) {
            $user = Auth::user();
            if ($user->dark_mode) {
                $primaryColor =  config('settings.dark_mode.primary_color');
                $secondaryColor = config('settings.dark_mode.secondary_color');
            } else {
                $primaryColor = $user->primary_color;
                $secondaryColor = $user->secondary_color;
            }
            $view->with('primaryColorValue', $primaryColor);
            $view->with('secondaryColorValue', $secondaryColor);
        });
    }
}
