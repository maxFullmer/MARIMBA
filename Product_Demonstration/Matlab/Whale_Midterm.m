%Whale Midterm for bonous fun

function varargout = Whale_Midterm(varargin)
% WHALE_MIDTERM MATLAB code for Whale_Midterm.fig
%      WHALE_MIDTERM, by itself, creates a new WHALE_MIDTERM or raises the existing
%      singleton*.
%
%      H = WHALE_MIDTERM returns the handle to a new WHALE_MIDTERM or the handle to
%      the existing singleton*.
%
%      WHALE_MIDTERM('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in WHALE_MIDTERM.M with the given input arguments.
%
%      WHALE_MIDTERM('Property','Value',...) creates a new WHALE_MIDTERM or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before Whale_Midterm_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to Whale_Midterm_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help Whale_Midterm

% Last Modified by GUIDE v2.5 05-Dec-2017 22:28:56

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @Whale_Midterm_OpeningFcn, ...
                   'gui_OutputFcn',  @Whale_Midterm_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before Whale_Midterm is made visible.
function Whale_Midterm_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to Whale_Midterm (see VARARGIN)

% Choose default command line output for Whale_Midterm
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes Whale_Midterm wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = Whale_Midterm_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

t = (1:1:100000)';

Longs = [];
Lats = [];

dir_content = dir('~/Desktop/1/*.txt');
filenames = {dir_content.name};
current_files = filenames;


while true

  pause(1);
  dir_content = dir('~/Desktop/1/*.txt');
  filenames = {dir_content.name};
  new_files = setdiff(filenames,current_files);
  
  if ~isempty(new_files)
    
    % deal with the new files
    fprintf('INCOMING TRANSMISSION!!\n')
    current_files = filenames;
    f = char(new_files);
    
     d = strcat('~/Desktop/1/', f);
%     fprintf('%s\n', d);
    pause(2);
    D = importdata(d, ',', 5);
    
    if isstruct(D)
      z = D.data(:,1);
    else
     z = D(:,1);
    end    
    
%    z = D.data(:,1);

    fileID = fopen(d,'r');
    Intro = textscan(fileID,'%s',4,'Delimiter','\n','Delimiter','l');
    V = [Intro{1,:}];

    Long = str2double(cell2mat(V(2,1:1)));   %41.489748
    Lat = str2double(cell2mat(V(3,1:1)));   %-71.422390

    Longs(end+1) = Long;
    Lats(end+1) = Lat;
    
    rng default

    dv = mean(z);
    x = z - dv;
    q = interp(x, 1);

    sound(q, 10000) %%Unfiltered Sound
    
    axes(handles.axes2);
    plot(t, z);


    Fs = 10000;
    Hd = designfilt('lowpassfir','FilterOrder',20,'CutoffFrequency',2000, ...
           'DesignMethod','window','Window',{@kaiser,3},'SampleRate',Fs);

    y1 = filter(Hd,x);

    axes(handles.axes1);
    plot(t,x,t,y1);
    xlabel('Time (s)')
    ylabel('Amplitude')
    legend('Original Signal','Filtered Data')

%    sound(y1, 10000)  %%Filtered Sound
%    pause(10);

    f = figure;
    movegui(f,'southeast');
    hold on
    plot(Lats(1:end-1), Longs(1:end-1),'or','LineWidth',3, ...
        'MarkerSize', 10, 'MarkerEdgeColor','k','MarkerFaceColor','r');
    plot(Lats(end), Longs(end),'^r','LineWidth',3, ...
        'MarkerSize', 10, 'MarkerEdgeColor','k','MarkerFaceColor','r');  
    hold off
    set(gca,'ydir','normal','fontsize',12,'fontname','times')
    xlabel('Longitude (^o West)')
    ylabel('Latitude (^o North)')
    plot_google_map('maptype','satellite','APIKey',...
        'AIzaSyDuxqsb-5TyavEgHUdhQn62zDtH6q4tQmI')
    disp(findobj('type','figure'))
    pause(10);
    
   else
     fprintf('No new files!\n')
  end
 end


% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
keyboard



function edit1_Callback(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit1 as text
%        str2double(get(hObject,'String')) returns contents of edit1 as a double


% --- Executes during object creation, after setting all properties.
function edit1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit2_Callback(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit2 as text
%        str2double(get(hObject,'String')) returns contents of edit2 as a double


% --- Executes during object creation, after setting all properties.
function edit2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
