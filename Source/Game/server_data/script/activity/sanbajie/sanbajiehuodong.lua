--���˸�Ů�ڻ

--sat_sun = 0
act_start = 0         --38�ڿ�ʼ����Ϊ1����������Ϊ2
FuNvJieLingQuRen = {}
PanAn = {}
MuLanHua = {}

function SanBaJie_OnTimerMin(actID)
    local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curmin = os.date("%M")
	local curhour = tonumber(os.date("%H"))
	if curmon ==3 and (curday >= 1 and curday <= 15) then
	    if act_start == 0 then
		    FuNvJieLingQuRen[1] = map.MapCreateCreature(3017298127, -1, 4900443, 2159, 6646, 2201)
			FuNvJieLingQuRen[2] = map.MapCreateCreature(3017299663, -1, 4900443, 2081, 18345, 2649)
		    PanAn[1] = map.MapCreateCreature(3017298127, -1, 4900444, 2153, 6646, 2201)
			PanAn[2] = map.MapCreateCreature(3017299663, -1, 4900444, 2076, 18345, 2649)
		    MuLanHua[1] = map.MapCreateCreature(3017298127, -1, 4900445, 2153, 6646, 2207)
			MuLanHua[2] = map.MapCreateCreature(3017299663, -1, 4900445, 2076, 18345, 2655)
			act_start = 1
		elseif act_start == 2 then
			map.MapDeleteCreature(3017298127, -1, PanAn[1])
			map.MapDeleteCreature(3017299663, -1, PanAn[2])
			FuNvJieLingQuRen[1] = map.MapCreateCreature(3017298127, -1, 4900443, 2159, 6646, 2201)
			FuNvJieLingQuRen[2] = map.MapCreateCreature(3017299663, -1, 4900443, 2081, 18345, 2649)
		    PanAn[1] = map.MapCreateCreature(3017298127, -1, 4900444, 2153, 6646, 2201)
			PanAn[2] = map.MapCreateCreature(3017299663, -1, 4900444, 2076, 18345, 2649)
		    MuLanHua[1] = map.MapCreateCreature(3017298127, -1, 4900445, 2153, 6646, 2207)
			MuLanHua[2] = map.MapCreateCreature(3017299663, -1, 4900445, 2076, 18345, 2655)
		    act_start = 1
		end
		if curmin == "00" and curhour % 2 == 0 then
		    -- �㲥��Ů�ڻ
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 2401)    --��Ʒ�����������ഫ�������°˺�������20�����ϵ���Ҷ��������������Ǽ��Ƶ۳ǵĸ�Ů����Ʒ��ȡ�˴���ȡ�������ѵ�������ƷŶ��
			msg.DispatchWorldMsgEvent(MsgID)
		end
	else
	    if act_start == 1 then
		    map.MapDeleteCreature(3017298127, -1, FuNvJieLingQuRen[1])
			map.MapDeleteCreature(3017299663, -1, FuNvJieLingQuRen[2])
			map.MapDeleteCreature(3017298127, -1, PanAn[1])
			map.MapDeleteCreature(3017299663, -1, PanAn[2])
			map.MapDeleteCreature(3017298127, -1, MuLanHua[1])
			map.MapDeleteCreature(3017299663, -1, MuLanHua[2])
			act_start = 0
		end
		local day = os.date("%A")
		if (day == "Saturday" or day == "Sunday") and act_start == 0 then
			PanAn[1] = map.MapCreateCreature(3017298127, -1, 4900444, 2153, 6646, 2201)
			PanAn[2] = map.MapCreateCreature(3017299663, -1, 4900444, 2076, 18345, 2649)
			act_start = 2
		elseif (day ~= "Saturday" and day ~= "Sunday") and act_start == 2 then
		    map.MapDeleteCreature(3017298127, -1, PanAn[1])
			map.MapDeleteCreature(3017299663, -1, PanAn[2])
			act_start = 0
		end
	end
end

--ע��û������¼�
aux.RegisterActEvent(78, 4, "SanBaJie_OnTimerMin")


