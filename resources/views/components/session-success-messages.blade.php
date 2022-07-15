@if (session('success'))
<div {{ $attributes }}>
    <div class="font-medium text-green-600">
        {{ session('success') }}
    </div>
</div>
@endif