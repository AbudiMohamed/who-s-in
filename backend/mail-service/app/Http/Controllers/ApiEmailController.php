<?php

namespace App\Http\Controllers;

use App\Mail\MailTemplate;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Mail;
use App\Exceptions\InvalidOrderException;

class ApiEmailController extends Controller
{
    public function sendEmail(Request $request)
    {
        $request->validate([
          'email' => 'required|email',
          'subject' => 'required',
          'name' => 'required',
          'content' => 'required',
        ]);

        $data = [
          'subject' => $request->subject,
          'name' => $request->name,
          'email' => $request->email,
          'content' => $request->content
        ];
        try{
            Mail::send('emails.template', $data, function($message) use ($data) {
                $message->to($data['email'])
                ->subject($data['subject']);
              });
              return response()->json(['status' => 'Email Sent'],200);
        }catch (Throwable $e){
            return response()->json(['status' => $e],404);
        }



        
    }
}
