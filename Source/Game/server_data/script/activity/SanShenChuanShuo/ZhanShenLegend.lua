
--���ڱ������ڱ��λ���Ƿ�����ɹ������ȫ��table�������RoleIDΪ��������Ӧ��¼��ͬ���
ZhanShenLegend_Quest_IsComplete = {};
--��¼ÿ�λ��������ɵ��ܴ���
ZhanShenLegend_CompleteQuestTime = 0;
--�������б�ˢ���ĻNPC�����ս�񡱵�CreatureID
ZhanShen_CreatureID = {};
--�����ܹ����ж��ٸ�NPC��ˢ��
ZhanShen_Quantity = 3;
--����ÿ����ˢ����NPC����Ϣ
ZhanShen_CreatureID[1] = {MapID=3017298127,x=2184,y=6940,z=2289,CreatureID=0};	--����:[2184,6940,2289]
ZhanShen_CreatureID[2] = {MapID=3017298383,x=888,y=10464,z=2033,CreatureID=0};	--����:[888,10464,2033]
ZhanShen_CreatureID[3] = {MapID=3017299663,x=2105,y=18338,z=2698,CreatureID=0};--��¹:[2105,18338,2698]

ZhanShenLegend_TimerMin = 0;		--��¼������˶೤ʱ��

--ս��˵�����������ڸ�����������������������⽱��
ZhanShen_Reward = {};
ZhanShen_Reward[20229]={Exp=5751,Gold=675};
ZhanShen_Reward[20230]={Exp=9165,Gold=1200};
ZhanShen_Reward[20231]={Exp=14777,Gold=1875};
ZhanShen_Reward[20232]={Exp=24024,Gold=2700};
ZhanShen_Reward[20233]={Exp=38902,Gold=3675};
ZhanShen_Reward[20234]={Exp=63892,Gold=4800};
ZhanShen_Reward[20235]={Exp=108787,Gold=6075};
ZhanShen_Reward[20236]={Exp=167014,Gold=7500};
ZhanShen_Reward[20237]={Exp=232394,Gold=9075};
ZhanShen_Reward[20238]={Exp=287856,Gold=10800};
ZhanShen_Reward[20239]={Exp=316843,Gold=12675};
ZhanShen_Reward[20240]={Exp=333937,Gold=14700};
ZhanShen_Reward[20241]={Exp=348498,Gold=16875};
ZhanShen_Reward[20242]={Exp=404271,Gold=19200};
ZhanShen_Reward[20243]={Exp=560003,Gold=21675};
ZhanShen_Reward[20244]={Exp=721481,Gold=24300};
ZhanShen_Reward[20245]={Exp=820832,Gold=27075};
ZhanShen_Reward[20323]={Exp=1020832,Gold=37075};
ZhanShen_Reward[20324]={Exp=1270832,Gold=47075};

function ZhanShenLegend_OnStart(actID)	--���ʼ

	--��ʼ���ȫ�ֱ���
	ZhanShenLegend_TimerMin = 0;
	ZhanShenLegend_CompleteQuestTime = 0;

	--ˢ���NPC ���ս��[3610204]
	for i=1,ZhanShen_Quantity do
		ZhanShen_CreatureID[i].CreatureID = map.MapCreateCreature(ZhanShen_CreatureID[i].MapID, -1, 3610204, ZhanShen_CreatureID[i].x, ZhanShen_CreatureID[i].y, ZhanShen_CreatureID[i].z);
	end

	--���ʼ���Ź㲥��
	--������ս��˵���ʼ����λ��ҿ����������ǡ����踮���Ƶ۳����ҵ�"���ս��"��ȡ�������ɺ���з��Ľ�������
	local MsgID = msg.BeginMsgEvent();
	if WorldCup_On== 0 then
		msg.AddMsgEvent(MsgID,102,100047)
	else
		msg.AddMsgEvent(MsgID,102,100120)
	end
	msg.DispatchBroadcast(MsgID,-1,-1,-1);
end



function ZhanShenLegend_OnTimerMin(actID)	--�ÿ����1����
	ZhanShenLegend_TimerMin = ZhanShenLegend_TimerMin+1;
	local isRestart = 0;
	for i=1,ZhanShen_Quantity do
		if ZhanShen_CreatureID[i].CreatureID == 0 then
			ZhanShen_CreatureID[i].CreatureID = map.MapCreateCreature(ZhanShen_CreatureID[i].MapID, -1, 3610204, ZhanShen_CreatureID[i].x, ZhanShen_CreatureID[i].y, ZhanShen_CreatureID[i].z);
			isRestart = isRestart+1;
			if isRestart==3 then
				local MsgID = msg.BeginMsgEvent();
				if WorldCup_On== 0 then
					msg.AddMsgEvent(MsgID,102,100047)
				else
					msg.AddMsgEvent(MsgID,102,100120)
				end
				msg.DispatchBroadcast(MsgID,-1,-1,-1);
				isRestart = 0;
			end
		end
	end
	if ZhanShenLegend_TimerMin==5 then
		local MsgID = msg.BeginMsgEvent();
		msg.AddMsgEvent(MsgID,102,100048);
		msg.DispatchBroadcast(MsgID,-1,-1,-1);
		for i=1,ZhanShen_Quantity do
			if  ZhanShen_CreatureID[i].CreatureID ~= nil then
				cre.MonsterSay(ZhanShen_CreatureID[i].MapID, -1, ZhanShen_CreatureID[i].CreatureID, 20124)
			end
		end
	elseif ZhanShenLegend_TimerMin==10 then
		local MsgID = msg.BeginMsgEvent();
		msg.AddMsgEvent(MsgID,102,100048);
		msg.DispatchBroadcast(MsgID,-1,-1,-1);
	elseif ZhanShenLegend_TimerMin==50 then
	--����е�50����ʱ���Ź㲥��
	--�����ս�񡱽���10���Ӻ󷵻���ͥ����Ҫ�������������뾡��ǰ������������ʱ���ռ���´λ���������
		local MsgID = msg.BeginMsgEvent();
		msg.AddMsgEvent(MsgID,102,100060);
		msg.AddMsgEvent(MsgID,5,3610204);
		msg.AddMsgEvent(MsgID,9,10);
		msg.DispatchBroadcast(MsgID,-1,-1,-1);
	elseif ZhanShenLegend_TimerMin==55 then
	--����е�55����ʱ
	--���Ź㲥��
	--�����ս�񡱽���5���Ӻ󷵻���ͥ����Ҫ�������������뾡��ǰ������������ʱ���ռ���´λ���������
	--���ս�񺰻���
	--����������������縴�����������ڱ�����ȥǰ�������񣬱�ֻ�ܵȱ����ٴ��·���ռ���»ص������޶��ˣ���
		local MsgID = msg.BeginMsgEvent();
		msg.AddMsgEvent(MsgID,102,100060);
		msg.AddMsgEvent(MsgID,5,3610204);
		msg.AddMsgEvent(MsgID,9,5);
		msg.DispatchBroadcast(MsgID,-1,-1,-1);
		for i=1,ZhanShen_Quantity do
			if  ZhanShen_CreatureID[i].CreatureID ~= nil then
				cre.MonsterSay(ZhanShen_CreatureID[i].MapID, -1, ZhanShen_CreatureID[i].CreatureID, 20120)
			end
		end
	--�ڻ�ĵ�10-55�����ڼ䣬ÿ3-7�������ս��һ�����������[20123 ~ 20126]�м����ѡ��һ�����ݽ��к���
	elseif ZhanShenLegend_TimerMin>10 and ZhanShenLegend_TimerMin<55 and math.mod(ZhanShenLegend_TimerMin,math.random(3,7))==0 then
		local r = math.random(1,4);
		if r==1 then
			for i=1,ZhanShen_Quantity do
				if  ZhanShen_CreatureID[i].CreatureID ~= nil then
					cre.MonsterSay(ZhanShen_CreatureID[i].MapID, -1, ZhanShen_CreatureID[i].CreatureID, 20123)
				end
			end
		elseif r==2 then
			for i=1,ZhanShen_Quantity do
				if  ZhanShen_CreatureID[i].CreatureID ~= nil then
					cre.MonsterSay(ZhanShen_CreatureID[i].MapID, -1, ZhanShen_CreatureID[i].CreatureID, 20124)
				end
			end
		elseif r==3 then
			for i=1,ZhanShen_Quantity do
				if  ZhanShen_CreatureID[i].CreatureID ~= nil then
					cre.MonsterSay(ZhanShen_CreatureID[i].MapID, -1, ZhanShen_CreatureID[i].CreatureID, 20125)
				end
			end
		elseif r==4 then
			for i=1,ZhanShen_Quantity do
				if  ZhanShen_CreatureID[i].CreatureID ~= nil then
					cre.MonsterSay(ZhanShen_CreatureID[i].MapID, -1, ZhanShen_CreatureID[i].CreatureID, 20126)
				end
			end
		end
	end
end



function ZhanShenLegend_OnEnd(actID)	--�����

	--�жϻNPC�Ƿ��Ѵﵽ����ʱ���Զ���ʧ����δ��ʧ���ڳ�����ɾ����NPC�������ͷ�NPC��CreatureID
	for i=1,ZhanShen_Quantity do
		if ZhanShen_CreatureID[i].CreatureID ~= nil and unit.IsDead(ZhanShen_CreatureID[i].MapID,-1,ZhanShen_CreatureID[i].CreatureID)==false then
			map.MapDeleteCreature(ZhanShen_CreatureID[i].MapID,-1,ZhanShen_CreatureID[i].CreatureID);
			ZhanShen_CreatureID[i].CreatureID = nil;
		else
			ZhanShen_CreatureID[i].CreatureID = nil;
		end
	end

	ZhanShenLegend_Quest_IsComplete = {};
	ZhanShenLegend_CompleteQuestTime = 0;
	ZhanShenLegend_TimerMin = 0;
	--��������Ź㲥��
	--������ս��˵��Ѿ���������δ����������ҿɼ����������񣬲����´λʱ�������񣡡�
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100049);
	msg.DispatchBroadcast(MsgID,-1,-1,-1);
end

function ZhanShenLegend_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)	--����������
	ZhanShenLegend_Quest_IsComplete[RoleID] = 1;
	ZhanShenLegend_CompleteQuestTime = ZhanShenLegend_CompleteQuestTime + 1;

	if WorldCup_On==1 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3304100, 1, -1, 8, 420)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 71, 3270)		--���籭�ڼ�����������Ž���
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID, 13, 0);
	msg.AddMsgEvent(MsgID, 1, 2730);
	msg.AddMsgEvent(MsgID, 11, QuestID);
	if ZhanShenLegend_CompleteQuestTime==1 then	--��ҵ�һ��������񣬶�����������1������ͽ�Ǯ����
		role.AddRoleExp(MapID, InstanceID, RoleID, ZhanShen_Reward[QuestID].Exp);
		role.AddRoleSilver(MapID, InstanceID, RoleID, ZhanShen_Reward[QuestID].Gold, 102);
		local MsgID_1 = msg.BeginMsgEvent();
		local MsgID_2 = msg.BeginMsgEvent();
		msg.AddMsgEvent(MsgID_1, 13, 0);
		msg.AddMsgEvent(MsgID_1, 1, 2721);
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1);
		msg.AddMsgEvent(MsgID_2, 102, 100050);
		msg.AddMsgEvent(MsgID_2, 2, RoleID);
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1);
		msg.AddMsgEvent(MsgID, 9, ZhanShen_Reward[QuestID].Exp*2);
		msg.AddMsgEvent(MsgID, 9, ZhanShen_Reward[QuestID].Gold*2);
		--���ӻ����ʵ��ֵ
		Increase_shili(MapID, InstanceID, RoleID, 0, 12,"jinjieshili_A")
	elseif ZhanShenLegend_CompleteQuestTime==10 then	--��ҵ�10��������񣬶�����������0.5������ͽ�Ǯ����
		local ex = math.floor(ZhanShen_Reward[QuestID].Exp/2);
		local go = math.floor(ZhanShen_Reward[QuestID].Gold/2);
		role.AddRoleExp(MapID, InstanceID, RoleID, ex);
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102);
		local MsgID_1 = msg.BeginMsgEvent();
		local MsgID_2 = msg.BeginMsgEvent();
		msg.AddMsgEvent(MsgID_1, 13, 0);
		msg.AddMsgEvent(MsgID_1, 1, 2722);
		msg.AddMsgEvent(MsgID_1, 9, 10);
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1);
		msg.AddMsgEvent(MsgID_2, 102, 100051);
		msg.AddMsgEvent(MsgID_2, 2, RoleID);
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1);
		msg.AddMsgEvent(MsgID, 9, ZhanShen_Reward[QuestID].Exp+ex);
		msg.AddMsgEvent(MsgID, 9, ZhanShen_Reward[QuestID].Gold+go);
		--���ӻ����ʵ��ֵ
		Increase_shili(MapID, InstanceID, RoleID, 0, 9,"jinjieshili_A")
	elseif ZhanShenLegend_CompleteQuestTime==100 then	--��ҵ�100��������񣬶�����������0.5������ͽ�Ǯ����
		local ex = math.floor(ZhanShen_Reward[QuestID].Exp/2);
		local go = math.floor(ZhanShen_Reward[QuestID].Gold/2);
		role.AddRoleExp(MapID, InstanceID, RoleID, ex);
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102);
		local MsgID_1 = msg.BeginMsgEvent();
		local MsgID_2 = msg.BeginMsgEvent();
		msg.AddMsgEvent(MsgID_1, 13, 0);
		msg.AddMsgEvent(MsgID_1, 1, 2722);
		msg.AddMsgEvent(MsgID_1, 9, 100);
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1);
		msg.AddMsgEvent(MsgID_2, 102, 100052);
		msg.AddMsgEvent(MsgID_2, 2, RoleID);
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1);
		msg.AddMsgEvent(MsgID, 9, ZhanShen_Reward[QuestID].Exp+ex);
		msg.AddMsgEvent(MsgID, 9, ZhanShen_Reward[QuestID].Gold+go);
		--���ӻ����ʵ��ֵ
		Increase_shili(MapID, InstanceID, RoleID, 0, 9,"jinjieshili_A")
	else
		msg.AddMsgEvent(MsgID, 9, ZhanShen_Reward[QuestID].Exp);
		msg.AddMsgEvent(MsgID, 9, ZhanShen_Reward[QuestID].Gold);
		--���ӻ����ʵ��ֵ
		Increase_shili(MapID, InstanceID, RoleID, 0, 6,"jinjieshili_A")
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID);
	--��ҽ�������ʱ��NPC��50%�������[20121 ~ 20122]�����һ������
	local r = math.random(1,4);
	if r==1 then
		for i=1,ZhanShen_Quantity do
			if ZhanShen_CreatureID[i].MapID == MapID and ZhanShen_CreatureID[i].CreatureID ~= nil then
				cre.MonsterSay(ZhanShen_CreatureID[i].MapID, -1, ZhanShen_CreatureID[i].CreatureID, 20121);
			end
		end
	elseif r==2 then
		for i=1,ZhanShen_Quantity do
			if ZhanShen_CreatureID[i].MapID == MapID and ZhanShen_CreatureID[i].CreatureID ~= nil then
				cre.MonsterSay(ZhanShen_CreatureID[i].MapID, -1, ZhanShen_CreatureID[i].CreatureID, 20122);
			end
		end
	end

end


function ZhanShenLegend_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if ZhanShenLegend_Quest_IsComplete[RoleID] == 1 then
		local MsgID_1 = msg.BeginMsgEvent();
		local MsgID_2 = msg.BeginMsgEvent();
		msg.AddMsgEvent(MsgID_1, 26, 2723);
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1);
		msg.AddMsgEvent(MsgID_2, 26, 2724);
		msg.DispatchRoleMsgEvent(RoleID, MsgID_2);

		return 0;
	end

	return 1;
end

function ZhanShenLegend_OnAccept(MapID, InstanceID, QuestID, RoleID, NPCID)

	ZhanShenLegend_Quest_IsComplete[RoleID] = 1;

end

function ZhanShenLegend_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 3+WorldCup_On) then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 3250)--��Ҫx�����ҿռ�
		msg.AddMsgEvent(MsgID, 9, 3+WorldCup_On)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end

--[[________________________ע������¼�________________________]]--

--���ʼ
aux.RegisterActEvent(111, 2, "ZhanShenLegend_OnStart")
aux.RegisterActEvent(112, 2, "ZhanShenLegend_OnStart")
aux.RegisterActEvent(113, 2, "ZhanShenLegend_OnStart")
aux.RegisterActEvent(114, 2, "ZhanShenLegend_OnStart")

--�����
aux.RegisterActEvent(111, 3, "ZhanShenLegend_OnEnd")
aux.RegisterActEvent(112, 3, "ZhanShenLegend_OnEnd")
aux.RegisterActEvent(113, 3, "ZhanShenLegend_OnEnd")
aux.RegisterActEvent(114, 3, "ZhanShenLegend_OnEnd")

--��ԡ����ӡ�Ϊ��λ��ʱ
aux.RegisterActEvent(111, 4, "ZhanShenLegend_OnTimerMin")
aux.RegisterActEvent(112, 4, "ZhanShenLegend_OnTimerMin")
aux.RegisterActEvent(113, 4, "ZhanShenLegend_OnTimerMin")
aux.RegisterActEvent(114, 4, "ZhanShenLegend_OnTimerMin")

--��ҽ�ȡ
aux.RegisterQuestEvent(20229, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20230, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20231, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20232, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20233, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20234, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20235, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20236, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20237, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20238, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20239, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20240, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20241, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20242, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20243, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20244, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20245, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20323, 0, "ZhanShenLegend_OnAccept")
aux.RegisterQuestEvent(20324, 0, "ZhanShenLegend_OnAccept")

--�����ɡ�ս��˵������
aux.RegisterQuestEvent(20229, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20230, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20231, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20232, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20233, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20234, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20235, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20236, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20237, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20238, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20239, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20240, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20241, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20242, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20243, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20244, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20245, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20323, 1, "ZhanShenLegend_OnComplete")
aux.RegisterQuestEvent(20324, 1, "ZhanShenLegend_OnComplete")

--�������Ƿ�ɽ�ȡ��ս��˵������
aux.RegisterQuestEvent(20229, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20230, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20231, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20232, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20233, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20234, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20235, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20236, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20237, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20238, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20239, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20240, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20241, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20242, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20243, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20244, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20245, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20323, 4, "ZhanShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(20324, 4, "ZhanShenLegend_OnCheckAccept")

--�������Ƿ�ɽ�ȡ��ս��˵������
aux.RegisterQuestEvent(20229, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20230, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20231, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20232, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20233, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20234, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20235, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20236, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20237, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20238, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20239, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20240, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20241, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20242, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20243, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20244, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20245, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20323, 5, "ZhanShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(20324, 5, "ZhanShenLegend_OnCheckComplete")
