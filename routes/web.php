<?php

use Illuminate\Support\Facades\Route;
use App\Http\Controllers\WelcomeController;
use App\Http\Controllers\Dashboard\{
    HomeController,
    ColorController
};

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', [WelcomeController::class, 'show'])->name('welcome');

Route::group([
    'prefix' => '/dashboard',
    'middleware' => ['auth'],
    'as' => 'dashboard.'
], function () {
    Route::get('/home', [HomeController::class, 'index'])->name('home');
    Route::get('/colors', [ColorController::class, 'create'])->name('colors.create');
    Route::post('/colors', [ColorController::class, 'store'])->name('colors.store');
});

require __DIR__ . '/auth.php';
