

--�����ڻ����
Chongyang_Creature = {}
Chongyang_Creature[1]={typeid=3610232,x=2222,y=6646,z=2222,creid=0}
Chongyang_Creature[2]={typeid=3610233,x=2389,y=10295,z=3413,creid=0}
Chongyang_Creature[3]={typeid=3610234,x=1272,y=4971,z=2794,creid=0}
Chongyang_Creature[4]={typeid=3610235,x=847,y=3856,z=1950,creid=0}
Chongyang_Creature[5]={typeid=3610236,x=940,y=7370,z=407,creid=0}
Chongyang_Creature[6]={typeid=3610237,x=2465,y=3832,z=1051,creid=0}
Chongyang_Creature[7]={typeid=3610238,x=2389,y=10285,z=3423,creid=0}
Chongyang_Creature[8]={typeid=3610239,x=1278,y=4980,z=2802,creid=0}
Chongyang_Creature[9]={typeid=3610240,x=855,y=3907,z=1955,creid=0}
Chongyang_Creature[10]={typeid=3610241,x=930,y=7287,z=410,creid=0}
Chongyang_Creature[11]={typeid=3610242,x=2464,y=3819,z=1059,creid=0}
Chongyang_Creature[12]={typeid=3610243,x=2184,y=8095,z=2355,creid=0}
Chongyang_Creature[13]={typeid=3610244,x=2184,y=10775,z=2377,creid=0}
Chongyang_Creature[14]={typeid=3610245,x=2183,y=14167,z=2418,creid=0}

--����ID ��20331-20335

function Chongyang_OnTimerMin(actID)

	if  Chongyangjie == 1 then
		local curmon = tonumber(os.date("%m"))
		local curday = tonumber(os.date("%d"))
		local curhour = tonumber(os.date("%H")) -- ��ȡ��������ǰСʱ��
		local curmin = tonumber(os.date("%M"))  -- ��ȡ��������ǰ������
		for i = 1, 12 do
			if  Chongyang_Creature[i].creid == 0 then
				Chongyang_Creature[i].creid =  map.MapCreateColCreature(3017298127, -1, Chongyang_Creature[i].typeid, Chongyang_Creature[i].x, Chongyang_Creature[i].y, Chongyang_Creature[i].z, 1)
			end
		end
		-- ÿ��18���ʼ���μ����������������ɾ�� �·����ˡ��� �� �·����ˡ���
		if curhour == 18 and curmin == 0 then
			local k = jizurenwushuliang
			if k ~= 0 then
				jizurenwushuliang = 0
			end
			for i = 13, 14 do
				if Chongyang_Creature[i] ~= 0 then
					local MsgID = msg.BeginMsgEvent() -- ���繫��
					msg.AddMsgEvent(MsgID, 100, 4122) -- �·����ˡ��� �� �·����ˡ��� �Ѿ��뿪���ǣ�ͨ����ɽ��μ�����������ٴν������ٻ�����
					msg.DispatchWorldMsgEvent(MsgID)
					map.MapDeleteCreature(3017298127, -1, Chongyang_Creature[i].creid)
					Chongyang_Creature[i].creid = 0
				end
			end
		end
	end
end

function Chongyang_OnStart(actID)
	if Chongyangjie == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 4123)	--�����ڻ�Ѿ���ʼ�ˣ����λ��ҵ����ǣ�2222,2222���ҵ�������ʹ���˽����ݡ�
		msg.DispatchWorldMsgEvent(MsgID)
	end
end

function Chongyang_OnEnd(actID)
	if Chongyangjie == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 4124)	--�����ڻ�Ѿ������ˣ���л��ҵĻ������롣
		msg.DispatchWorldMsgEvent(MsgID)
		for i = 1 , 12 do
			if  Chongyang_Creature[i].creid ~= 0 then
				map.MapDeleteCreature(3017298127, -1, Chongyang_Creature[i].creid)
				Chongyang_Creature[i].creid =0
			end
		end
		Chongyangjie = 0
	end
end

--ע��û������¼�
aux.RegisterActEvent(154, 4, "Chongyang_OnTimerMin")	--�ID
aux.RegisterActEvent(154, 2, "Chongyang_OnStart")	--�ID
aux.RegisterActEvent(154, 3, "Chongyang_OnEnd")		--�ID
















