
--Ѱ��ɢ�ɻ
--3011066	����ʦ
--3011067	����ʦ
--3011068	����ʦ
--3011069	½��ʦ

--��������������������ˢ��NPC
sanxian_pos={}
--����
sanxian_pos[1]={MapID=3017298127,x=2033,y=4963,z=2136}
sanxian_pos[2]={MapID=3017298127,x=2314,y=6646,z=2247}
sanxian_pos[3]={MapID=3017298127,x=2022,y=6647,z=2442}
--����
sanxian_pos[4]={MapID=3017298383,x=717,y=10000,z=2267}
sanxian_pos[5]={MapID=3017298383,x=846,y=10000,z=1996}
sanxian_pos[6]={MapID=3017298383,x=1090,y=10000,z=2172}
--��¹
sanxian_pos[7]={MapID=3017299663,x=2023,y=18338,z=2377}
sanxian_pos[8]={MapID=3017299663,x=2149,y=17512,z=2604}
sanxian_pos[9]={MapID=3017299663,x=2145,y=19209,z=2817}
--ۺ��
sanxian_pos[10]={MapID=3017299919,x=878,y=20140,z=1011}
sanxian_pos[11]={MapID=3017299919,x=1128,y=20140,z=831}
sanxian_pos[12]={MapID=3017299919,x=1394,y=20372,z=968}
--��Ҷ
sanxian_pos[13]={MapID=3017299151,x=2242,y=6744,z=1807}
sanxian_pos[14]={MapID=3017299151,x=2024,y=6365,z=1725}
--�����칬
sanxian_pos[15]={MapID=3017299407,x=1826,y=14024,z=2358}
sanxian_pos[16]={MapID=3017299407,x=1912,y=14596,z=2567}
sanxian_pos[17]={MapID=3017299407,x=1841,y=15007,z=2778}

function xunzhaosanxian_OnStart(actID)
	local   k = math.random(1,17)
	--�ڸ����������������ѡ��һ�������ˢ��ɢ��
	local n = math.random(1,100)
	if n >= 1 and n<=90 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 412)--ɢ��###��NPC���ƣ��½�Ѳ����###��������Ӧ�ĳ������ƣ�����λ����ɢ�ɴ��������������Ʒ
		msg.AddMsgEvent(MsgID, 5, 3011066)
		msg.AddMsgEvent(MsgID, 6, sanxian_pos[k].MapID)
		msg.DispatchWorldMsgEvent(MsgID)
		map.MapCreateColCreature(sanxian_pos[k].MapID, -1, 3011066, sanxian_pos[k].x, sanxian_pos[k].y, sanxian_pos[k].z, 1)
	elseif n >= 91 and n<=95 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 412)--ɢ��###��NPC���ƣ��½�Ѳ����###��������Ӧ�ĳ������ƣ�����λ����ɢ�ɴ��������������Ʒ
		msg.AddMsgEvent(MsgID, 5, 3011068)
		msg.AddMsgEvent(MsgID, 6, sanxian_pos[k].MapID)
		msg.DispatchWorldMsgEvent(MsgID)
		map.MapCreateColCreature(sanxian_pos[k].MapID, -1, 3011068, sanxian_pos[k].x, sanxian_pos[k].y, sanxian_pos[k].z, 1)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 412)--ɢ��###��NPC���ƣ��½�Ѳ����###��������Ӧ�ĳ������ƣ�����λ����ɢ�ɴ��������������Ʒ
		msg.AddMsgEvent(MsgID, 5, 3011069)
		msg.AddMsgEvent(MsgID, 6, sanxian_pos[k].MapID)
		msg.DispatchWorldMsgEvent(MsgID)
		map.MapCreateColCreature(sanxian_pos[k].MapID, -1, 3011069, sanxian_pos[k].x, sanxian_pos[k].y, sanxian_pos[k].z, 1)
	end

end

--ע��û������¼�
aux.RegisterActEvent(81, 2, "xunzhaosanxian_OnStart")-----�ID
aux.RegisterActEvent(82, 2, "xunzhaosanxian_OnStart")-----�ID
aux.RegisterActEvent(83, 2, "xunzhaosanxian_OnStart")-----�ID
aux.RegisterActEvent(84, 2, "xunzhaosanxian_OnStart")-----�ID
aux.RegisterActEvent(85, 2, "xunzhaosanxian_OnStart")-----�ID
aux.RegisterActEvent(91, 2, "xunzhaosanxian_OnStart")-----�ID















