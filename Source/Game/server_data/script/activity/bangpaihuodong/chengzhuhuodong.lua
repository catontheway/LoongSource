--�����
Liedimo_dead = 0
liedimoid = 0
Liedimo_times = {}

chengzhu_pos = {}
chengzhu_pos[1] = {x= 825,y= 10350,z= 606}--�ѵ�ħ����
chengzhu_pos[2] = {x= 815,y= 10415,z= 578}--�ѵ�ħ
chengzhu_pos[3] = {x= 685,y= 10364,z= 538}--�ѵ�ħ����
chengzhu_pos[4] = {x= 682,y= 10382,z= 522}--�ѵ�ħ���
chengzhu_pos[5] = {x= 671,y= 10401,z= 530}--�ѵ�ħ���
chengzhu_pos[6] = {x= 673,y= 10401,z= 548}--�ѵ�ħ���
chengzhu_pos[7] = {x= 692,y= 10372,z= 545}--�ѵ�ħ���
chengzhu_pos[8] = {x= 701,y= 10408,z= 535}--�ѵ�ħ���
chengzhu_pos[9] = {x= 526,y= 10375,z= 570}--�ѵ�ħ����
chengzhu_pos[10] = {x= 524,y= 10400,z= 561}--�ѵ�ħ���
chengzhu_pos[11] = {x= 518,y= 10400,z= 567}--�ѵ�ħ���
chengzhu_pos[12] = {x= 521,y= 10400,z= 573}--�ѵ�ħ���
chengzhu_pos[13] = {x= 525,y= 10400,z= 579}--�ѵ�ħ���
chengzhu_pos[14] = {x= 532,y= 10369,z= 575}--�ѵ�ħ���
chengzhu_pos[15] = {x= 531,y= 10350,z= 569}--�ѵ�ħ����
chengzhu_pos[16] = {x= 525,y= 10350,z= 700}--�ѵ�ħ���
chengzhu_pos[17] = {x= 525,y= 10350,z= 691}--�ѵ�ħ���
chengzhu_pos[18] = {x= 518,y= 10394,z= 697}--�ѵ�ħ���
chengzhu_pos[19] = {x= 519,y= 10393,z= 708}--�ѵ�ħ���
chengzhu_pos[20] = {x= 530,y= 10350,z= 712}--�ѵ�ħ���
chengzhu_pos[21] = {x= 904,y= 10400,z= 561}--�ѵ�ħ����
chengzhu_pos[22] = {x= 905,y= 10388,z= 579}--�ѵ�ħ���
chengzhu_pos[23] = {x= 913,y= 10400,z= 570}--�ѵ�ħ���
chengzhu_pos[24] = {x= 908,y= 10400,z= 557}--�ѵ�ħ���
chengzhu_pos[25] = {x= 895,y= 10362,z= 554}--�ѵ�ħ���
chengzhu_pos[26] = {x= 892,y= 10387,z= 562}--�ѵ�ħ���
chengzhu_pos[27] = {x= 927,y= 10350,z= 694}--�ѵ�ħ����
chengzhu_pos[28] = {x= 927,y= 10350,z= 683}--�ѵ�ħ���
chengzhu_pos[29] = {x= 918,y= 10356,z= 689}--�ѵ�ħ���
chengzhu_pos[30] = {x= 921,y= 10371,z= 701}--�ѵ�ħ���
chengzhu_pos[31] = {x= 934,y= 10350,z= 696}--�ѵ�ħ���
chengzhu_pos[32] = {x= 934,y= 10386,z= 688}--�ѵ�ħ���

function liedimogenban_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    local curhour = tonumber(os.date("%H"))
	local Role_GuildID = guild.GetRoleGuildID(RoleID)
	local Master_GuilID = city.GetCityAppointedAtt(3, 2)
	local Role_ZhiWei = -1
	if Role_GuildID ~= nil and Role_GuildID ~= 4294967295 then
		Role_ZhiWei = guild.GetRoleGuildPos(Role_GuildID, RoleID)
	end
	if TalkIndex == -1 then
	    if Liedimo_dead == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	3428)		--���ѵ�ħ���ࣺ\n    �С����н����ǳ��������������ϡ����ϴ�Ҫ�̡�����ѵ��ѵ�����𡭡����ⶢ�����쿴�����Ĵ�ү�пе����촽����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			if Role_GuildID ~= -1 and Role_GuildID ~= 4294967295 and Role_GuildID == Master_GuilID and Role_ZhiWei == 2 and liedimoid == 0 then
				msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 3429)			--���Ҿ��ǳ�����
			end
			msg.AddMsgEvent(MsgID, 21,	18)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    if Liedimo_times[RoleID] ==nil then
			    Liedimo_times[RoleID] = 0
			end
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	3430+Liedimo_times[RoleID])		--����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 3442)			--������һ�š�
			msg.AddMsgEvent(MsgID, 21,	18)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

	elseif TalkIndex == 4 then
        if Role_GuildID ~= -1 and Role_GuildID ~= 4294967295 and Role_GuildID == Master_GuilID and Role_ZhiWei == 2 and liedimoid == 0 then
			liedimoid = map.MapCreateColCreature(3695619387, -1, 1534162, chengzhu_pos[2].x, chengzhu_pos[2].y, chengzhu_pos[2].z, 1, "")
		    for i= 1,5 do
			    map.MapCreateColCreature(3695619387, -1, 1534163, chengzhu_pos[2+6*(i-1)+1].x, chengzhu_pos[2+6*(i-1)+1].y, chengzhu_pos[2+6*(i-1)+1].z, 1, "")
		        map.MapCreateColCreature(3695619387, -1, 1534164, chengzhu_pos[3+6*(i-1)+1].x, chengzhu_pos[3+6*(i-1)+1].y, chengzhu_pos[3+6*(i-1)+1].z, 1, "")
				map.MapCreateColCreature(3695619387, -1, 1534164, chengzhu_pos[4+6*(i-1)+1].x, chengzhu_pos[3+6*(i-1)+1].y, chengzhu_pos[3+6*(i-1)+1].z, 1, "")
				map.MapCreateColCreature(3695619387, -1, 1534164, chengzhu_pos[5+6*(i-1)+1].x, chengzhu_pos[3+6*(i-1)+1].y, chengzhu_pos[3+6*(i-1)+1].z, 1, "")
				map.MapCreateColCreature(3695619387, -1, 1534164, chengzhu_pos[6+6*(i-1)+1].x, chengzhu_pos[3+6*(i-1)+1].y, chengzhu_pos[3+6*(i-1)+1].z, 1, "")
				map.MapCreateColCreature(3695619387, -1, 1534164, chengzhu_pos[7+6*(i-1)+1].x, chengzhu_pos[3+6*(i-1)+1].y, chengzhu_pos[3+6*(i-1)+1].z, 1, "")
			end
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 3443)  --ֻ�г������Թ����ѵ�ħ����������ҿ��Ի�ɱħ�������������ѵ�ħ��������
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 3443)  --ֻ�г������Թ����ѵ�ħ����������ҿ��Ի�ɱħ�������������ѵ�ħ��������
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		end
	elseif TalkIndex == 5 then
	    local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 and Liedimo_times[RoleID]~= nil and Liedimo_times[RoleID] < 10 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	3444)		--������������ñ�����������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	-1)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif Liedimo_times[RoleID]~= nil and Liedimo_times[RoleID] < 11 then
		    Liedimo_times[RoleID] = Liedimo_times[RoleID] + 1
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	3430+Liedimo_times[RoleID])		--����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	3442)			--������һ�š�
			msg.AddMsgEvent(MsgID, 21,	18)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			if Liedimo_times[RoleID] <= 5 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 3302302, 1, -1, 8, 420)
		    elseif Liedimo_times[RoleID] == 10 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 3302302, 5, -1, 8, 420)
			end
		elseif Liedimo_times[RoleID]~= nil and Liedimo_times[RoleID] >= 10 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	3430+11)		--����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	3442)			--������һ�š�
			msg.AddMsgEvent(MsgID, 21,	18)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end

end

aux.RegisterCreatureEvent(4900709, 7, "liedimogenban_OnTalk")

function I3302302_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	role.AddRoleExp(MapID, InstanceID, TargetID, math.floor(activity_newexp[rolelevel]/10))
end

aux.RegisterItemEvent(3302302, 1, "I3302302_GiftBag")

function szhaohuanmojing_Cast(Mapid, Instanceid, Skillid, Ownerid)
	local x, y ,z = unit.GetPosition(Mapid, Instanceid, Ownerid)
	map.MapCreateColCreature(3695619387, -1, 1534165, x, y, z, 1, "")
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 3445)  --��Ѹ�ٻ�ɱ����ħ���������ѵ�ħ�����ָ���������
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 3445)  --��Ѹ�ٻ�ɱ����ħ���������ѵ�ħ�����ָ���������
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	return 0
end


aux.RegisterSkillEvent(2429301, 1, "szhaohuanmojing_Cast")
aux.RegisterSkillEvent(2429401, 1, "szhaohuanmojing_Cast")
aux.RegisterSkillEvent(2429501, 1, "szhaohuanmojing_Cast")

--��������
function C1534162_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    Liedimo_dead = 1
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 3446)  --��������ɹ���ɱ�ѵ�ħ�������й�ս��һ�þ��齱����
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	map.MapCreateColCreature(3695619387, -1, 4900710, chengzhu_pos[2].x, chengzhu_pos[2].y, chengzhu_pos[2].z, 1, "")
	local RoleTbl = map.GetMapAllRoleID(3695619387, -1)
	for k,v in pairs(RoleTbl) do
		local rolelevel = role.GetRoleLevel(3695619387, -1, v)
		role.AddRoleExp(3695619387, -1, v, activity_newexp[rolelevel])
	end
end
aux.RegisterCreatureEvent(1534162, 4, "C1534162_OnDie")

--��������
function C1534163_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    unit.AddBuff(MapID, InstanceID, liedimoid, 5004501, liedimoid)
end
aux.RegisterCreatureEvent(1534163, 4, "C1534163_OnDie")

function c4900709_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    Liedimo_times = {}
	liedimoid = 0
	Liedimo_dead = 0
end

aux.RegisterCreatureEvent(4900709, 13, "c4900709_OnDisappear")

function c1534165_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    if Liedimo_dead == 0 then
		unit.AddBuff(MapID, InstanceID, liedimoid, 5004601, liedimoid)
		--[[local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, liedimoid)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 3416)  --�����ڷ��衾xxx��xxx���ٻ��˷��踮����������ǰȥ������ɻ��ȡ���飡
		msg.AddMsgEvent(MsgID, 9, liedimoid)
		msg.AddMsgEvent(MsgID, 9, TypeID)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)]]
	end
end

aux.RegisterCreatureEvent(1534165, 13, "c1534165_OnDisappear")

--��������
function I4900710_OnInVest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	local sex = role.GetRoleSex(RoleID)
	if sex == 1 then
		local Totle = {8971016,8971024,8971030}
		local index = math.random(3)
		role.AddRoleItem(MapID, InstanceID, RoleID, Totle[index], 1, 1, 8, 420)
		table.remove(Totle, index)
		index = math.random(3)
		role.AddRoleItem(MapID, InstanceID, RoleID, Totle[index], 1, 1, 8, 420)
	elseif sex == 0 then
		local Totle = {8971016,8971024,8971030}
		local index = math.random(3)
		role.AddRoleItem(MapID, InstanceID, RoleID, Totle[index], 1, 1, 8, 420)
		table.remove(Totle, index)
		index = math.random(3)
		role.AddRoleItem(MapID, InstanceID, RoleID, Totle[index], 1, 1, 8, 420)
	end

end
aux.RegisterCreatureEvent(4900710, 6, "I4900710_OnInVest")
